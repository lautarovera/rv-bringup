/*
 * platform.h - QEMU "virt" machine memory map (RV64)
 *
 * Source of truth: the device tree QEMU generates.
 *   qemu-system-riscv64 -machine virt,dumpdtb=virt.dtb
 *   dtc -I dtb -O dts virt.dtb
 */
#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>

/* DRAM */
#define DRAM_BASE           0x80000000UL
#define DRAM_SIZE           0x08000000UL   /* 128 MiB default */

/* NS16550A UART */
#define UART0_BASE          0x10000000UL
#define UART0_IRQ           10             /* PLIC source */

/* SiFive test finisher: lets firmware end the QEMU run (used by CI) */
#define TEST_FINISHER_BASE  0x00100000UL
#define FINISHER_PASS       0x5555
#define FINISHER_FAIL       0x3333         /* | (exit_code << 16) */

/* CLINT (timer + software interrupts) */
#define CLINT_BASE          0x02000000UL
#define CLINT_MSIP(h)       (CLINT_BASE + 0x0000UL + 4UL * (h))
#define CLINT_MTIMECMP(h)   (CLINT_BASE + 0x4000UL + 8UL * (h))
#define CLINT_MTIME         (CLINT_BASE + 0xBFF8UL)
#define MTIME_FREQ_HZ       10000000UL     /* 10 MHz on QEMU virt */

/* PLIC */
#define PLIC_BASE           0x0C000000UL

/* PCIe host bridge (pci-host-ecam-generic) */
#define PCIE_ECAM_BASE      0x30000000UL   /* 256 MiB = 256 buses */
#define PCIE_MMIO32_BASE    0x40000000UL   /* 32-bit BAR window */
#define PCIE_MMIO32_SIZE    0x40000000UL
#define PCIE_PIO_BASE       0x03000000UL
#define PCIE_INTX_IRQ_BASE  32             /* INTA..INTD -> PLIC 32..35 (swizzled) */

/* QEMU "edu" teaching device: run QEMU with -device edu */
#define EDU_VENDOR_ID       0x1234
#define EDU_DEVICE_ID       0x11e8

/* MMIO helpers */
static inline void     mmio_w8 (uintptr_t a, uint8_t  v) { *(volatile uint8_t  *)a = v; }
static inline uint8_t  mmio_r8 (uintptr_t a)             { return *(volatile uint8_t  *)a; }
static inline void     mmio_w32(uintptr_t a, uint32_t v) { *(volatile uint32_t *)a = v; }
static inline uint32_t mmio_r32(uintptr_t a)             { return *(volatile uint32_t *)a; }
static inline void     mmio_w64(uintptr_t a, uint64_t v) { *(volatile uint64_t *)a = v; }
static inline uint64_t mmio_r64(uintptr_t a)             { return *(volatile uint64_t *)a; }

#endif /* PLATFORM_H */
