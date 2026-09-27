/*
 * uart.c - polled NS16550A driver (QEMU virt UART0)
 */
#include "uart.h"
#include "platform.h"

#define REG_THR   0x00   /* transmit holding (write) */
#define REG_RBR   0x00   /* receive buffer (read) */
#define REG_IER   0x01
#define REG_FCR   0x02
#define REG_LCR   0x03
#define REG_LSR   0x05
#define LSR_DR    0x01   /* data ready */
#define LSR_THRE  0x20   /* THR empty */

void uart_init(void)
{
    /* QEMU does not need baud setup, real 16550s do (DLAB + divisor). */
    mmio_w8(UART0_BASE + REG_IER, 0x00);   /* interrupts off (M4 turns RX on) */
    mmio_w8(UART0_BASE + REG_FCR, 0x07);   /* enable + clear FIFOs */
    mmio_w8(UART0_BASE + REG_LCR, 0x03);   /* 8N1 */
}

void uart_putc(char c)
{
    while (!(mmio_r8(UART0_BASE + REG_LSR) & LSR_THRE)) {
        __asm__ volatile ("nop");
    }

    mmio_w8(UART0_BASE + REG_THR, (uint8_t)c);
}

void uart_puts(const char *s)
{
    while (*s) {
        if (*s == '\n')
            uart_putc('\r');
        uart_putc(*s++);
    }
}

void uart_puthex(uint64_t v)
{
    static const char hex[] = "0123456789abcdef";
    uart_puts("0x");
    for (int i = 60; i >= 0; i -= 4) {
        uart_putc(hex[(v >> i) & 0xf]);
    }
}

int uart_getc_nonblock(void)
{
    if (mmio_r8(UART0_BASE + REG_LSR) & LSR_DR) {
        return mmio_r8(UART0_BASE + REG_RBR);
    }

    return -1;
}
