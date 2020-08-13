#include "logo.h"
#include "uart.h"

void kernel_main(void) {
  init_uart();
  print_moss_logo();
}
