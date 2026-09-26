/*
 * link.ld - single-image layout at the start of DRAM (QEMU virt, -bios none)
 */
OUTPUT_ARCH("riscv")
ENTRY(_start)

MEMORY
{
    RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

STACK_SIZE = 16K;

SECTIONS
{
    .text : {
        KEEP(*(.text.entry))
        *(.text .text.*)
    } > RAM

    .rodata : ALIGN(8) {
        *(.rodata .rodata.* .srodata .srodata.*)
    } > RAM

    .data : ALIGN(8) {
        *(.data .data.*)
        __global_pointer$ = . + 0x800;
        *(.sdata .sdata.*)
    } > RAM

    .bss (NOLOAD) : ALIGN(8) {
        __bss_start = .;
        *(.sbss .sbss.* .bss .bss.* COMMON)
        . = ALIGN(8);
        __bss_end = .;
    } > RAM

    .stack (NOLOAD) : ALIGN(16) {
        . += STACK_SIZE;
        __stack_top = .;
    } > RAM

    /DISCARD/ : { *(.eh_frame .comment .riscv.attributes) }
}
