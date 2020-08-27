#include "hal/el.h"
#include "hal/init.h"
#include "hal/random.h"
#include "log.h"
#include "logo.h"

void kernel_main(void) {
  init_board();

  print_moss_logo();

  for (int i = 0; i < 10; ++i) {
    log_d("test random: %d", generate_random(0, 100));
  }

  log_i("current el: %d", get_current_el());
  log_d("kernel main done");
}
