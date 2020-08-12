#include "log.h"
#include "logo.h"
#include "uart.h"

void kernel_main(void) {
  init_uart();
  print_moss_logo();

  log_i("hello %d", 123);
  log_i("hello %s", "world");
  log_i("hello 0x%x", 12);
  log_i("hello 0X%X", 12);
}
