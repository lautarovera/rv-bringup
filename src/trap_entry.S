/*
 * trap_entry.S - M1: trap entry/exit (TODO)
 *
 * Suggested shape:
 *     .align 2
 *     .globl trap_vector
 * trap_vector:
 *     addi sp, sp, -FRAME_SIZE
 *     sd   ra, 0(sp) ... save x1..x31 (skip x0, handle sp carefully)
 *     mv   a0, sp
 *     call trap_handler
 *     ld   ... restore
 *     addi sp, sp, FRAME_SIZE
 *     mret
 *
 * Later (M3): when S-mode code traps into M-mode, sp belongs to S-mode.
 * Consider swapping to a dedicated M-mode stack via the mscratch CSR.
 */
