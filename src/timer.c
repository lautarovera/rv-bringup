/*
 * timer.c - M2: machine timer interrupt via CLINT
 *
 * TODO (M2):
 *  1. Program mtimecmp[hart0] = mtime + MTIME_FREQ_HZ / 100   (10 ms)
 *  2. Enable MTIE in mie and MIE in mstatus.
 *  3. In the trap handler, on mcause = interrupt | 7, count the tick and
 *     re-arm mtimecmp (writing mtimecmp is what clears the pending bit).
 *  4. Self-test: wait until N ticks arrive (wfi in a loop), check the elapsed
 *     mtime is roughly N * period, disable the timer, return MS_OK.
 *
 * Depends on: M1.
 */
#include <stdint.h>
#include "milestones.h"
#include "platform.h"
#include "csr.h"
#include "uart.h"
#include "timer.h"

static volatile uint64_t ticks;   /* written by timer_interrupt, read by m2_timer */

/* Called from trap_handler on a machine timer interrupt */
void timer_interrupt(void)
{
    ticks++;
    mmio_w64(CLINT_MTIMECMP(0), UINT64_MAX);   /* disarm: never fire again, clears MTIP */
}

int m2_timer(void)
{
    uint64_t start = mmio_r64(CLINT_MTIME);
    mmio_w64(CLINT_MTIMECMP(0), start + MTIME_FREQ_HZ / 100);   /* 1. arm: 10 ms from now */

    csr_set(mie, MIE_MTIE);                                     /* 2. enable the timer interrupt */
    csr_set(mstatus, MSTATUS_MIE);                              /* 3. enable interrupts globally */

    uint64_t timeout = start + MTIME_FREQ_HZ / 10;              /* give up after 100 ms */
    while (ticks == 0 && mmio_r64(CLINT_MTIME) < timeout) {
        /* wait for the interrupt */
    }

    csr_clear(mstatus, MSTATUS_MIE);                            /* interrupts off again */
    csr_clear(mie, MIE_MTIE);

    uart_puts("m2: ticks = ");
    uart_puthex(ticks);
    uart_puts(", elapsed ticks = ");
    uart_puthex(mmio_r64(CLINT_MTIME) - start);
    uart_puts("\n");

    return MS_TODO;
}
