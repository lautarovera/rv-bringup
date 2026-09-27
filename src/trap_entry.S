/*
 * trap_entry.S - M1: trap entry/exit (TODO)
 *
 * Current (M1): trap_vector saves and restores the 16 caller-saved registers.
 * Between the save and restore, it calls to the C trap handler (trap.c) and
 * updates the mepc CSR to the next instruction.
 *
 * Later (M3): when S-mode code traps into M-mode, sp belongs to S-mode.
 * Consider swapping to a dedicated M-mode stack via the mscratch CSR.
 */
    .section .text
    .align 2
    .globl trap_vector
trap_vector:
    /* 1. Save: reserve the frame and store the 16 caller-saved registers */
    addi    sp, sp, -128
    sd      ra,   0(sp)
    sd      t0,   8(sp)
    sd      t1,  16(sp)
    sd      t2,  24(sp)
    sd      t3,  32(sp)
    sd      t4,  40(sp)
    sd      t5,  48(sp)
    sd      t6,  56(sp)
    sd      a0,  64(sp)
    sd      a1,  72(sp)
    sd      a2,  80(sp)
    sd      a3,  88(sp)
    sd      a4,  96(sp)
    sd      a5, 104(sp)
    sd      a6, 112(sp)
    sd      a7, 120(sp)

    /* 2. Call C, then skip the trapping instruction (still +4 for now) */
    call    trap_handler
    csrr    t0, mepc
    addi    t0, t0, 4
    csrw    mepc, t0

    /* 3. Restore: same offsets, then release the frame and return */
    ld      ra,   0(sp)
    ld      t0,   8(sp)
    ld      t1,  16(sp)
    ld      t2,  24(sp)
    ld      t3,  32(sp)
    ld      t4,  40(sp)
    ld      t5,  48(sp)
    ld      t6,  56(sp)
    ld      a0,  64(sp)
    ld      a1,  72(sp)
    ld      a2,  80(sp)
    ld      a3,  88(sp)
    ld      a4,  96(sp)
    ld      a5, 104(sp)
    ld      a6, 112(sp)
    ld      a7, 120(sp)
    addi    sp, sp, 128
    mret
