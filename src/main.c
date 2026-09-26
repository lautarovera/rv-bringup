/*
 * main.c - bring-up sequence
 *
 * Each milestone reports its own status line. CI (tests/run_qemu.sh) checks
 * those lines, so the log doubles as the bring-up checklist.
 */
#include <stdint.h>
#include "platform.h"
#include "csr.h"
#include "uart.h"
#include "milestones.h"

static void print_misa(void)
{
    uint64_t misa = csr_read(misa);
    uart_puts("misa: RV64");
    for (int i = 0; i < 26; i++)
        if (misa & (1UL << i))
            uart_putc((char)('A' + i));
    uart_puts("\n");
}

static void finish(int ok)
{
    mmio_w32(TEST_FINISHER_BASE, ok ? FINISHER_PASS : (FINISHER_FAIL | (1u << 16)));
    for (;;)
        __asm__ volatile ("wfi");
}

static int report(const char *name, int rc)
{
    uart_puts(name);
    if (rc == MS_OK)            uart_puts(": OK\n");
    else if (rc == MS_TODO)     uart_puts(": TODO\n");
    else                        uart_puts(": FAIL\n");
    return rc;
}

int main(void)
{
    uart_init();
    uart_puts("\nrv-bringup: bare-metal RISC-V on QEMU virt\n");
    uart_puts("hart: ");  uart_puthex(csr_read(mhartid)); uart_puts("\n");
    print_misa();
    uart_puts("mtime: "); uart_puthex(mmio_r64(CLINT_MTIME)); uart_puts("\n");
    report("[M0] boot + uart", MS_OK);

    int failed = 0;
    failed |= report("[M1] traps",               m1_traps())           == MS_FAIL;
    failed |= report("[M2] timer interrupt",     m2_timer())           == MS_FAIL;
    failed |= report("[M3] pmp + s-mode",        m3_pmp_smode())       == MS_FAIL;
    failed |= report("[M4] plic + uart rx irq",  m4_plic())            == MS_FAIL;
    failed |= report("[M5] pcie enumeration",    m5_pcie_enumerate())  == MS_FAIL;
    failed |= report("[M6] pcie bar + mmio",     m6_pcie_bars())       == MS_FAIL;
    failed |= report("[M7] pcie dma + irq",      m7_pcie_dma_irq())    == MS_FAIL;

    uart_puts("done\n");
    finish(!failed);
    return 0;
}
