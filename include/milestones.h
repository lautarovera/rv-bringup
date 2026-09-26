/*
 * milestones.h - one entry point per bring-up milestone
 *
 * Return MS_OK when the milestone self-test passes, MS_TODO while it is not
 * implemented yet, MS_FAIL when the self-test ran and failed.
 */
#ifndef MILESTONES_H
#define MILESTONES_H

#define MS_OK    0
#define MS_TODO  1
#define MS_FAIL  2

int m1_traps(void);          /* src/trap.c   */
int m2_timer(void);          /* src/timer.c  */
int m3_pmp_smode(void);      /* src/smode.c  */
int m4_plic(void);           /* src/plic.c   */
int m5_pcie_enumerate(void); /* src/pcie.c   */
int m6_pcie_bars(void);      /* src/pcie.c   */
int m7_pcie_dma_irq(void);   /* src/edu.c    */

#endif /* MILESTONES_H */
