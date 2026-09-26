/*
 * trap.c - M1: machine-mode trap handling
 *
 * Goal: any exception or interrupt lands in one handler that prints mcause,
 * mepc and mtval, and can return to the faulting code when it makes sense.
 *
 * TODO (M1):
 *  1. Write trap_vector in src/trap_entry.S:
 *     - reserve a trap frame on the stack, save all caller-saved + callee-saved GPRs
 *     - call trap_handler(frame), restore registers, mret
 *     - mtvec needs 4-byte alignment (direct mode, low bits = 0)
 *  2. Point mtvec at trap_vector (from start.S or m1_traps()).
 *  3. trap_handler(): decode mcause (interrupt bit + code), print mcause/mepc/mtval.
 *     For synchronous exceptions you want to skip, advance mepc by 4
 *     (watch out for compressed 2-byte instructions if you enable C).
 *  4. Self-test: trigger `ecall` (mcause 11) and an illegal instruction (mcause 2),
 *     check the handler saw both, then return MS_OK.
 *
 * Read: RISC-V Privileged Spec, "Machine Trap Vector Base Address" and "mcause".
 */
#include "milestones.h"

int m1_traps(void)
{
    return MS_TODO;
}
