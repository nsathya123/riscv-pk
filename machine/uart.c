// See LICENSE for license details.

#include <string.h>
#include "uart.h"
#include "fdt.h"

volatile uint32_t* uart;

#define STS_TX_FULL         1 << 1
#define STS_RX_NOT_EMPTY    1 << 2

void uart_putchar(uint8_t ch)
{
	volatile uint32_t *tx = uart + UART_REG_TXFIFO; 

	while (uart[UART_STATUS] & STS_TX_FULL);
	*tx = ch;
}

int uart_getchar()
{
	while((uart[UART_STATUS] & STS_RX_NOT_EMPTY) == 0); 
    
	uint8_t ch = uart[UART_REG_RXFIFO];

	if (ch <= 0 || ch>127) return -1;
	return ch;
}

struct uart_scan
{
  int compat;
  uint64_t reg;
};

static void uart_open(const struct fdt_scan_node *node, void *extra)
{
  struct uart_scan *scan = (struct uart_scan *)extra;
  memset(scan, 0, sizeof(*scan));
}

static void uart_prop(const struct fdt_scan_prop *prop, void *extra)
{
  struct uart_scan *scan = (struct uart_scan *)extra;
	
  if (!strcmp(prop->name, "compatible") && fdt_string_list_index(prop, "shakti,uart0") >= 0) {
    scan->compat = 1;
  } else  if (!strcmp(prop->name, "compatible") && fdt_string_list_index(prop, "shakti,uart1") >= 0) {
    scan->compat = 1;
  } else  if (!strcmp(prop->name, "compatible") && fdt_string_list_index(prop, "shakti,uart2") >= 0) {
    scan->compat = 1;
  } else if (!strcmp(prop->name, "reg")) {
    fdt_get_address(prop->node->parent, prop->value, &scan->reg);
  }
}

static void uart_done(const struct fdt_scan_node *node, void *extra)
{
  struct uart_scan *scan = (struct uart_scan *)extra;
  if (!scan->compat || !scan->reg || uart) return;

}

void query_uart(uintptr_t fdt)
{
  struct fdt_cb cb;
  struct uart_scan scan;

  memset(&cb, 0, sizeof(cb));
  cb.open = uart_open;
  cb.prop = uart_prop;
  cb.done = uart_done;
  cb.extra = &scan;

  fdt_scan(fdt, &cb);
}
