/*
 * start.S - reset entry (M-mode, -bios none)
 *
 * QEMU virt with -bios none jumps straight to 0x80000000 in M-mode on all harts.
 * Hart 0 sets up the C environment. Other harts park in wfi.
 */
    .section .text.entry
    .globl _start
_start:
    csrr    t0, mhartid
    bnez    t0, park

    /* No trap handler yet: point mtvec at a safe spin loop so an early
     * fault is visible in GDB instead of jumping to address 0.
     * M1: replace with the real trap vector (trap_vector in trap_entry.S). */
    la      t0, early_trap
    csrw    mtvec, t0

    /* Global pointer (linker relaxation) */
    .option push
    .option norelax
    la      gp, __global_pointer$
    .option pop

    la      sp, __stack_top

    /* Zero .bss */
    la      t0, __bss_start
    la      t1, __bss_end
1:  bgeu    t0, t1, 2f
    sd      zero, 0(t0)
    addi    t0, t0, 8
    j       1b
2:
    call    main

    /* main returned: stop */
park:
    wfi
    j       park

    .align 2
early_trap:
    j       early_trap
