# Design decisions

Short records of the choices made while bringing this platform up. One entry per decision: the context, the options, what was chosen and why. Keep each one under half a page.

## D1: Run bare metal with `-bios none` instead of booting under OpenSBI

- Context: QEMU virt can start firmware directly in M-mode, or load OpenSBI and start the payload in S-mode.
- Options: (a) own M-mode from reset, (b) run as an S-mode payload under OpenSBI.
- Decision: (a).
- Why: the point of the project is to own the M-mode layer, the trap path, PMP and the M to S handoff. That is the layer a platform firmware team writes. Booting under OpenSBI is a later step, once the M-mode pieces are understood.

## D2: Single C image, one hart

- Context: QEMU virt starts every hart at `0x80000000`.
- Decision: hart 0 runs, the rest park in `wfi`.
- Why: keeps the early milestones deterministic. Waking secondary harts with CLINT software interrupts is a good later milestone.

## D3: Warning on RWX regions

- Context: since binutils 2.39, ld warns when one loadable region is readable, writable and executable. That is a security risk under an OS.
- Options: (a) add -Wl,--no-warn-rwx-segments, (b) split code (RX) from data (RW) with PHDRS in link.ld file.
- Decision: (a).
- Why: do (a) for now, and (b) during M3. Everything runs in M-mode from a single RAM region with no memory protection active, so the warning carries no information until PMP exists in M3.

## D4: Trap frame saves only the caller-saved registers

- Context: on a trap, RISC-V hardware saves nothing to memory. It only writes `mepc`, `mcause`, `mtval` and the `mstatus` stack bits. The trap stub has to save whatever the C handler may overwrite before it calls it.
- Options: (a) save all 31 GPRs, (b) save only the 16 registers the psABI lets a C function clobber: `ra`, `t0`-`t6`, `a0`-`a7`.
- Decision: (b), in a 128-byte frame (16 x 8 bytes, which keeps `sp` 16-byte aligned as the psABI requires).
- Why: a C function must preserve `s0`-`s11` itself, so saving them in the stub would duplicate work. This is the same set a Cortex-M core stacks in hardware, which is why a Cortex-M handler can be a plain C function. Checked in GDB: `t0`, `t6` and `a5` survive a trap, and removing only the `a5` save/restore corrupts `a5`. The boot test still passed in that case, so GDB was the only way to see it.
- Revisit in M3: the mini SBI must read `a0`-`a7` from the frame and return a value in `a0`, so `trap_handler` will take a pointer to the frame. A trap from S-mode also arrives on the S-mode stack, so the stub will switch to an M-mode stack through `mscratch`.
- What I would do differently on real silicon: once an FPU or vector unit is on (`mstatus.FS`/`VS`), the handler must save that state too, or be built so it never touches it.

## D5: Direct-mode `mtvec`, installed at reset in `start.S`

- Context: the Privileged spec leaves `mtvec` unspecified at reset (QEMU leaves it at 0). Any fault before it is set jumps to an unknown address.
- Options: direct mode (every trap goes to BASE) or vectored mode (interrupts go to BASE + 4 x cause). Install it at the top of `start.S` or later, from `m1_traps()`.
- Decision: direct mode, installed by hart 0 before any C code runs.
- Why: vectored mode only speeds up dispatch of interrupts; exceptions always go to BASE. With at most three interrupt sources (timer, software, external) one handler that decodes `mcause` is simpler. Installing it first means a fault during C startup prints its cause instead of hanging.
- What I would do differently on real silicon: set `mtvec` on every hart before parking it. Here the secondary harts reach `wfi` with `mtvec` unset, which is harmless only because nothing ever wakes them. For interrupt-latency-sensitive parts, look at vectored mode or a CLIC.

## D6: Skip by the real instruction length, prove it with a landing flag

- Context: after an exception `mepc` points at the trapping instruction, and with the C extension it can be 2 or 4 bytes long. A first version that always added 4 skipped the 2-byte `li a0,1` after a compressed `unimp` and printed a false `[M1] traps: OK`.
- Decision: compute the length from the low 2 bits of the 16-bit halfword at `mepc` (`11` = 4 bytes, otherwise 2), and only for exceptions. For interrupts `mepc` is the next instruction to run and must not move.
- Why not `mtval`: the spec allows it to be 0, and for `c.unimp` the instruction bits are 0 anyway. Why a 16-bit read: a 4-byte instruction may sit at a 2-byte-aligned address, and a 32-bit load there would be misaligned.
- Self-test: `ecall`, a 2-byte and a 4-byte illegal instruction. The handler records `count`, `code` and `len` in a `volatile` struct. Each trapping instruction sits in one `asm` block followed by `li landed, 1`, with a `"memory"` clobber. Without the flag, a wrong skip jumped over the `li a2,2` that loads the check's expected length and made the check pass. With it, both broken variants (length always 4, length always 2) report FAIL.
- Known limitation: the handler skips every exception, including real faults such as access faults, so a bug can go unnoticed. Before M3's negative PMP test, unexpected exceptions should fail fast through the test finisher instead.

<!--
Template for new entries:

## Dn: <title>

- Context:
- Options:
- Decision:
- Why:
- What I would do differently on real silicon:
-->
