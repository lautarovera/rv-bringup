/*
 * trap_entry.S - M1: trap entry/exit (TODO)
 *
 * Current (M1): trap_vector uses t0 to hold the address to return to,
 * but in a real program a trap can happen in the middle of any calculation,
 * and the value would be silently corrupted.
 * TODO: the handler must save every register it touches and restore it before mret.
 *
 * Later (M3): when S-mode code traps into M-mode, sp belongs to S-mode.
 * Consider swapping to a dedicated M-mode stack via the mscratch CSR.
 */
    .section .text
    .align 2
    .globl trap_vector
trap_vector:
    csrr    t0, mepc        # t0 = address of the instruction that trapped (the ecall)
    addi    t0, t0, 4       # t0 = address of the next instruction (ecall is 4 bytes)
    csrw    mepc, t0        # mepc = t0
    mret                    # return: jump to the address in mepc
