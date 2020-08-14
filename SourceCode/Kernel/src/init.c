#include "log.h"
#include "logo.h"
#include "uart.h"
#include "utils.h"

void kernel_main(void) {
  init_uart();
  print_moss_logo();

  log_i("current el: %d", get_current_el());
}
