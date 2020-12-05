#include "hal/el.hpp"
#include "hal/init.hpp"
#include "kernel/logo.hpp"
#include "libcxx/log.hpp"

extern_C void kernel_main() {
  log_d("compiler info, %s", COMPILER_INFO);

  log_d(">>>>>>>>> entered kernel_main <<<<<<<<<");
  init_board();

  print_moss_logo();

  init_cpu();

  log_i("current el: %llu", get_current_el());
  double x = 5.0;
  double y = 3.0;
  double z = x / y;
  log_d("5/3=%f", z);

  //   unmapped virtual memory invalid access test
  //   u64 v = *(u64*)0x16;
  log_d(">>>>>>>>> exited kernel_main <<<<<<<<<");
  log_e("this line of code should never be executed");
}
