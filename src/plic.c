/*
 * plic.c - M4: external interrupts through the PLIC (UART RX)
 *
 * TODO (M4):
 *  1. PLIC: set priority of source UART0_IRQ (10) > 0, enable it for the
 *     M-mode context of hart 0, set that context's threshold to 0.
 *  2. UART: enable the "received data available" interrupt (IER bit 0).
 *  3. Enable MEIE in mie.
 *  4. Handler: claim (read the claim register), service the UART, complete
 *     (write the same id back).
 *  5. Self-test in CI: tests/run_qemu.sh can pipe a character into QEMU's stdin.
 *     Check the ISR received it.
 *
 * Read: RISC-V PLIC spec (context layout: hart0 M-mode is context 0 on virt).
 * Depends on: M1.
 */
#include "milestones.h"

int m4_plic(void)
{
    return MS_TODO;
}
