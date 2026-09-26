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
#include "milestones.h"

int m2_timer(void)
{
    return MS_TODO;
}
