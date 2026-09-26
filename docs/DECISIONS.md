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

<!--
Template for new entries:

## Dn: <title>

- Context:
- Options:
- Decision:
- Why:
- What I would do differently on real silicon:
-->
