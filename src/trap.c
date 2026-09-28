/*
 * trap.c - M1: machine-mode trap handling
 *
 * Goal: any exception or interrupt lands in one handler that prints mcause,
 * mepc and mtval, and can return to the faulting code when it makes sense.
 *
 * M1 (DONE):
 *  1. Write trap_vector in src/trap_entry.S:
 *     - reserve a trap frame on the stack, save all caller-saved registers
 *     - call trap_handler(void), restore registers, mret
 *     - mtvec needs 4-byte alignment (direct mode, low bits = 0)
 *  2. Point mtvec at trap_vector (from start.S).
 *  3. trap_handler(): decode mcause (interrupt bit + code), print mcause/mepc/mtval.
 *     For synchronous exceptions you want to skip, advance mepc by the real length, 2 or 4.
 *     C is enabled.
 *  4. Self-test with three traps: `ecall`, and illegal instructions of 2 and 4 bytes, each
 *     checked for count, cause, length and landing.
 *
 * Read: RISC-V Privileged Spec, "Machine Trap Vector Base Address" and "mcause".
 */
#include <stdint.h>
#include "milestones.h"
#include "csr.h"
#include "uart.h"
#include "timer.h"

/* What the last trap was: written by trap_handler, read by the self-test */
struct trap_record {
    uint64_t count;   /* number of traps seen */
    uint64_t code;    /* mcause code of the last trap */
    uint64_t len;     /* bytes skipped for the last exception */
};

static volatile struct trap_record record;

/* Length in bytes of the instruction at addr: 4 if the low 2 bits are 11, else 2 */
static uint64_t insn_length(uint64_t addr)
{
    uint16_t first_half = *(const uint16_t *)(uintptr_t)addr;
    return ((first_half & 0x3) == 0x3) ? 4 : 2;
}

void trap_handler(void)
{
    /* Read trap information from CSRs */
    uint64_t cause = csr_read(mcause);
    uint64_t epc   = csr_read(mepc);
    uint64_t tval  = csr_read(mtval);

    /* Decode the trap type */
    int is_interrupt = (cause & MCAUSE_INTERRUPT) != 0;
    uint64_t code    = cause & ~MCAUSE_INTERRUPT;

    /* Print trap information */
    uart_puts(is_interrupt ? "trap: interrupt" : "trap: exception");
    uart_puts(" code=");
    uart_puthex(code);
    uart_puts(" mepc=");
    uart_puthex(epc);
    uart_puts(" mtval=");
    uart_puthex(tval);
    uart_puts("\n");

    /* For synchronous exceptions, advance mepc by the length of the faulting instruction */
    uint64_t len = 0;
    if (!is_interrupt) {
        len = insn_length(epc);
        csr_write(mepc, epc + len);
    }

    /* Interrupts: hand them to the driver that owns the source */
    if (is_interrupt && code == IRQ_M_TIMER) {
        timer_interrupt();
    }

    /* Record the trap for the self-test */
    record.count++;
    record.code = code;
    record.len  = len;
}

/*
 * Run one trapping instruction. 'landed' is set to 1 only by the instruction
 * right after it, so it stays 0 if the handler skipped too far.
 */
#define TRAP_AND_LAND(insn, landed)          \
    __asm__ volatile ("li   %0, 0\n"         \
                      insn "\n"              \
                      "li   %0, 1\n"         \
                      : "=r"(landed) : : "memory")

/* True if the last trap matches what the self-test expects */
static int last_trap_is(uint64_t count, uint64_t code, uint64_t len)
{
    return record.count == count && record.code == code && record.len == len;
}

/* Self-test for machine-level traps */
int m1_traps(void)
{
    uint64_t landed;

    record.count = 0;

    TRAP_AND_LAND("ecall", landed);
    if (!landed || !last_trap_is(1, CAUSE_ECALL_M, 4)) {
        return MS_FAIL;
    }

    TRAP_AND_LAND("unimp", landed);                  /* illegal, 2 bytes */
    if (!landed || !last_trap_is(2, CAUSE_ILLEGAL_INSN, 2)) {
        return MS_FAIL;
    }

    TRAP_AND_LAND(".option push\n.option norvc\nunimp\n.option pop", landed);  /* illegal, 4 bytes */
    if (!landed || !last_trap_is(3, CAUSE_ILLEGAL_INSN, 4)) {
        return MS_FAIL;
    }

    return MS_OK;
}
