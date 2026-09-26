/*
 * smode.c - M3: PMP setup and drop from M-mode to S-mode
 *
 * TODO (M3):
 *  1. PMP: by default S/U-mode has no access to anything. Configure at least
 *     one region (pmpaddr0 + pmpcfg0, NAPOT or TOR) that gives S-mode RWX over
 *     DRAM and RW over the UART, and nothing else.
 *  2. Delegate selected exceptions/interrupts to S-mode (medeleg / mideleg) and
 *     set up stvec with an S-mode handler.
 *  3. Set mstatus.MPP = S, mepc = s_mode_entry, then mret.
 *  4. In S-mode: print a line, then trigger an `ecall` that goes back to M-mode
 *     (mcause 9, environment call from S-mode). This is your mini SBI.
 *  5. Negative test: touch an address outside the PMP region from S-mode and
 *     check M-mode sees an access fault (mcause 5 or 7).
 *
 * Talking point: this is the same split OpenSBI (M-mode) and Linux (S-mode) use.
 * Depends on: M1.
 */
#include "milestones.h"

int m3_pmp_smode(void)
{
    return MS_TODO;
}
