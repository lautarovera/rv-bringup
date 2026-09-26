/*
 * edu.c - M7: DMA and interrupts with the QEMU edu device
 *
 * Device reference: QEMU source, docs/specs/edu.rst
 *   0x80 DMA source, 0x88 DMA destination, 0x90 DMA count, 0x98 DMA command
 *   The device has a 4 KiB internal buffer at device address 0x40000.
 *   0x60 interrupt raise, 0x24 interrupt status, 0x64 interrupt acknowledge
 *
 * TODO (M7):
 *  1. Enable Bus Master in the Command register, otherwise DMA is blocked.
 *  2. DMA RAM -> device buffer -> RAM (two transfers), compare buffers.
 *     No IOMMU on virt by default, so bus address == physical address.
 *     Think about cache coherence: on QEMU it "just works", on real silicon
 *     you would need to clean/invalidate or use coherent memory.
 *  3. Interrupt on DMA completion: legacy INTx routed through the PLIC
 *     (PCIE_INTX_IRQ_BASE + swizzle based on device number and INTx pin).
 *     Read the Interrupt Pin register (config 0x3D) and compute the PLIC source.
 *  4. Stretch: MSI. Needs the AIA interrupt controllers:
 *     run QEMU with -machine virt,aia=aplic-imsic and program the MSI capability
 *     to write into the IMSIC. Document what you learned in docs/DECISIONS.md.
 *
 * Depends on: M4, M6.
 */
#include "milestones.h"

int m7_pcie_dma_irq(void)
{
    return MS_TODO;
}
