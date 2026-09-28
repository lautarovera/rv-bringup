/*
 * csr.h - minimal CSR access helpers
 */
#ifndef CSR_H
#define CSR_H

#include <stdint.h>

#define csr_read(csr) ({                                  \
    uint64_t __v;                                         \
    __asm__ volatile ("csrr %0, " #csr : "=r"(__v));      \
    __v; })

#define csr_write(csr, val) \
    __asm__ volatile ("csrw " #csr ", %0" :: "rK"(val))

#define csr_set(csr, bits) \
    __asm__ volatile ("csrs " #csr ", %0" :: "rK"(bits))

#define csr_clear(csr, bits) \
    __asm__ volatile ("csrc " #csr ", %0" :: "rK"(bits))

/* mstatus fields */
#define MSTATUS_MIE         (1UL << 3)
#define MSTATUS_MPIE        (1UL << 7)
#define MSTATUS_MPP_SHIFT   11
#define MSTATUS_MPP_MASK    (3UL << MSTATUS_MPP_SHIFT)

/* mie / mip bits */
#define MIE_MSIE            (1UL << 3)
#define MIE_MTIE            (1UL << 7)
#define MIE_MEIE            (1UL << 11)
#define MIP_MTIP            (1UL << 7)

/* mcause */
#define MCAUSE_INTERRUPT    (1UL << 63)
#define CAUSE_ILLEGAL_INSN  2
#define CAUSE_ECALL_M       11
#define IRQ_M_TIMER         7

#endif /* CSR_H */
