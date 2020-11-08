#include "hal/el.h"
#include "hal/init.h"
#include "kernel/logo.h"
#include "libc/log.h"

extern "C" void kernel_main(void) {
#ifdef __GNUC__
  log_d("gcc version: %s", __VERSION__);
#endif

  log_d(">>>>>>>>> entered kernel_main <<<<<<<<<");
  init_board();

  print_moss_logo();

  init_cpu();

  log_i("current el: %llu", get_current_el());
  double x = 5.0;
  double y = 3.0;
  double z = x / y;
  log_d("5/3=%f", z);

  // unmapped virtual memory invalid access test
  // u64 v = *(u64*)0x16;
  log_d(">>>>>>>>> exited kernel_main <<<<<<<<<");
}
