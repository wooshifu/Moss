#include "log.h"
#include "uart.h"

void kernel_main(void) {
  init_uart();
  const char *hello = "HelloWorld\n";
  while (*hello) {
    uart_put_char(*hello++);
  }
  const char *hello1 = "HelloWorldAgain\n";
  while (*hello1) {
    uart_put_char(*hello1++);
  }
  log_i("hello %d", 123);
  log_i("hello %s", "world");
  log_i("hello 0x%x", 12);
  log_i("hello 0X%X", 12);
  log_i("hello %p", hello);
}
