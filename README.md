# rv-bringup

Bare-metal RISC-V (RV64) platform bring-up on QEMU `virt`, from reset to PCIe DMA, with no OS, no SBI and no vendor SDK.

The goal is to walk the same path firmware takes on a new SoC: get a console alive, own the trap path, set up timers and interrupt controllers, hand off from M-mode to S-mode behind PMP, then enumerate PCIe, assign BARs and move data by DMA. Every step has a self-test that runs in CI.

## Status

| # | Milestone | What it proves | Status |
|---|-----------|----------------|--------|
| M0 | Boot + UART | Reset entry, stack, .bss, linker script, polled 16550 | Done |
| M1 | Traps | `mtvec`, trap frame, `mcause`/`mepc`/`mtval` decode, `mret` | TODO |
| M2 | Timer interrupt | CLINT `mtime`/`mtimecmp`, `mie`/`mstatus`, periodic tick | TODO |
| M3 | PMP + S-mode | PMP regions, `medeleg`/`mideleg`, M to S handoff, mini SBI via `ecall` | TODO |
| M4 | PLIC | External interrupts, claim/complete, UART RX IRQ | TODO |
| M5 | PCIe enumeration | ECAM config access, bus 0 scan, vendor/device/class | TODO |
| M6 | PCIe BARs | BAR sizing, 64-bit BARs, MMIO window allocation, Command register | TODO |
| M7 | PCIe DMA + IRQ | Bus mastering, DMA with the QEMU `edu` device, INTx through the PLIC (MSI via AIA as stretch) | TODO |
| M8 | Rust | Port one driver (UART or ECAM access) to `no_std` Rust and link it with the C image | TODO |

Update the table and uncomment the matching line in `tests/run_qemu.sh` when a milestone passes.

## Build and run

Requirements (Ubuntu 24.04 or WSL):

```
sudo apt install gcc-riscv64-unknown-elf qemu-system-misc
```

```
make          # build build/rv-bringup.elf
make run      # boot in QEMU (Ctrl-A X to quit)
make test     # boot, capture UART, check milestone lines (same as CI)
make debug    # QEMU halted, GDB on :1234
```

Expected output today:

```
rv-bringup: bare-metal RISC-V on QEMU virt
hart: 0x0000000000000000
misa: RV64ACDFHIMSU
mtime: 0x...
[M0] boot + uart: OK
[M1] traps: TODO
...
done
```

## Layout

```
link.ld              memory layout, stack, .bss symbols
include/platform.h   QEMU virt memory map (from the generated device tree)
include/csr.h        CSR helpers and bit definitions
src/start.S          reset entry, hart parking, C runtime setup
src/main.c           bring-up sequence and milestone report
src/uart.c           NS16550A polled driver
src/trap*.{c,S}      M1
src/timer.c          M2
src/smode.c          M3
src/plic.c           M4
src/pcie.c           M5, M6
src/edu.c            M7
tests/run_qemu.sh    boot test used by CI
docs/DECISIONS.md    design decisions and trade-offs
```

## Platform notes

The memory map comes from the device tree QEMU generates, not from memory:

```
qemu-system-riscv64 -machine virt,dumpdtb=virt.dtb -device edu
dtc -I dtb -O dts virt.dtb
```

Key addresses: DRAM `0x80000000`, UART0 `0x10000000` (PLIC source 10), CLINT `0x02000000`, PLIC `0x0C000000`, PCIe ECAM `0x30000000`, 32-bit MMIO window `0x40000000`, PCIe INTA..INTD on PLIC sources 32..35.

## What QEMU hides compared to real silicon

Worth knowing for disclaimer:

- No clock, PLL or DDR training. Real bring-up starts much earlier than `_start`.
- No PCIe link training (LTSSM). On hardware, a device that never leaves Detect or Polling is the first thing you debug.
- DMA is cache-coherent and there is no IOMMU by default.
- Timing is not cycle-accurate, so performance numbers from QEMU mean little.

## References

- RISC-V Privileged Architecture spec (traps, CSRs, PMP)
- RISC-V PLIC spec, RISC-V AIA spec (for MSI)
- PCI Express Base spec, config space and ECAM
- QEMU `docs/specs/edu.rst`
