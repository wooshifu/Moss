#include "hal/uart.hh"

constexpr auto DEBUG_UART = 1;

void platform_dputc(char ch) {
  if (ch == '\n') {
    uart_putc(DEBUG_UART, '\r');
  }
  uart_putc(DEBUG_UART, ch);
}
