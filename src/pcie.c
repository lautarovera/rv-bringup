/*
 * pcie.c - M5/M6: PCIe enumeration and BAR assignment over ECAM
 *
 * ECAM: config space of (bus, dev, fn) lives at
 *     PCIE_ECAM_BASE + (bus << 20) + (dev << 15) + (fn << 12) + offset
 *
 * TODO (M5, enumeration):
 *  1. ecam_read32(bus, dev, fn, off) / ecam_write32(...)
 *  2. Scan bus 0, dev 0..31, fn 0 (then multi-function via header type bit 7).
 *     Vendor ID 0xFFFF = nothing there.
 *  3. Print bus:dev.fn, vendor:device, class code for every function found.
 *  4. Self-test: the host bridge (1b36:0008) and the edu device (1234:11e8)
 *     must both be found. QEMU must run with `-device edu`.
 *
 * TODO (M6, BARs):
 *  1. BAR sizing: write 0xFFFFFFFF, read back, restore, size = ~(val & ~0xF) + 1.
 *     Handle 64-bit BARs (type bits = 0b10 take two slots).
 *  2. Allocate each memory BAR from the PCIE_MMIO32 window, naturally aligned.
 *  3. Enable Memory Space in the Command register (bit 1). Bus Master (bit 2)
 *     comes in M7.
 *  4. Self-test: read edu BAR0 + 0x00 (identification register) and check its
 *     low byte is 0xed. Then write a value to the factorial register (0x08),
 *     read back the result.
 *
 * Talking point: this is what firmware (u-boot/EDK2) or the kernel's PCI core
 * does before any driver probes. Real hardware adds link training (LTSSM)
 * before any of this works. QEMU skips that.
 */
#include "milestones.h"

int m5_pcie_enumerate(void)
{
    return MS_TODO;
}

int m6_pcie_bars(void)
{
    return MS_TODO;
}
