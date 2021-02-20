#include "hal/halt.hh"      // for never_return
#include "hal/init.hh"      // for init_board, init_cpu
#include "kconfig.hh"       // for CONFIG_BOARD_USIN...
#include "kernel/logo.hh"   // for print_moss_logo
#include "libcxx/log.hh"    // for log_i
#include "libcxx/macro.hh"  // for extern_C
#include "libcxx/printf.hh" // for printf

extern_C [[noreturn]] void kernel_main() {
#if CONFIG_BOARD_USING_RaspberryPi3
  log_i("CONFIG_BOARD_USING_RaspberryPi3");
#endif
  init_board();

  printf(">>>>>>>>> entered kernel_main <<<<<<<<<\n");

  print_moss_logo();

  init_cpu();

  log_i("Hello %s", "World");
  log_i("test int: %d", 123);
  log_i("test long long: %llu", 123LL);
  log_i("test float: %f", 1.123456789);
  printf(">>>>>>>>> exited kernel_main <<<<<<<<<\n");

  never_return();
  printf("this line of code should never be executed\n");
}
