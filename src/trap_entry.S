/*
 * trap_entry.S - M1: trap entry/exit (TODO)
 *
 * Current (M1): trap_vector saves and restores t0 using the stack to prevent
 * corruption of the interrupted code's t0.
 *
 * Later (M3): when S-mode code traps into M-mode, sp belongs to S-mode.
 * Consider swapping to a dedicated M-mode stack via the mscratch CSR.
 */
    .section .text
    .align 2
    .globl trap_vector
trap_vector:
    addi    sp, sp, -16     # reserve 16 bytes on the stack (16 bytes alignment rule)
    sd      t0, 0(sp)       # save the interrupted code's t0 on the stack
    csrr    t0, mepc        # t0 = address of the instruction that trapped (the ecall)
    addi    t0, t0, 4       # t0 = address of the next instruction (ecall is 4 bytes)
    csrw    mepc, t0        # mepc = t0
    ld      t0, 0(sp)       # restore the interrupted code's t0 from the stack
    addi    sp, sp, 16      # release the 16 bytes back to the stack
    mret                    # return: jump to the address in mepc
