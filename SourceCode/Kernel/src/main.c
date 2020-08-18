#include "hal/el.h"
#include "hal/init.h"
#include "log.h"
#include "logo.h"

void kernel_main(void) {
  init_board();

  print_moss_logo();

  log_i("current el: %d", get_current_el());
}
