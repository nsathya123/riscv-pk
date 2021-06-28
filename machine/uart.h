// See LICENSE for license details.

#ifndef _RISCV_UART_H
#define _RISCV_UART_H

#include <stdint.h>

extern volatile uint32_t* uart;

#define UART_REG_TXFIFO		4
#define UART_REG_RXFIFO		8
#define UART_REG_TXCTRL		2
#define UART_STATUS            0xC

#define UART_REG_RXCTRL		3
#define UART_REG_IE			4
#define UART_REG_IP			5
#define UART_REG_DIV			6

#define UART_TXEN		 0x1
#define UART_RXEN		 0x1

void uart_putchar(uint8_t ch);
int uart_getchar();
void query_uart(uintptr_t dtb);

#endif
