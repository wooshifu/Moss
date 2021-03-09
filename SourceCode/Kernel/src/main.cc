#include "hal/init.hh"           // for init_board_with_hooks, init_cpu, pos...
#include "hal/oops.hh"           // for oops
#include "kernel/logo.hh"        // for print_moss_logo
#include "libcxx/error_code.hh"  // for not_ok
#include "libcxx/log.hh"         // for log_i
#include "libcxx/macro.hh"       // for extern_C
#include "libcxx/printf.hh"      // for printf

extern_C [[noreturn]] void kernel_main() {
  pre_kernel_main();

  auto result = init_board_with_hooks();
  if (not_ok(result)) {
    oops();
  }
  printf(">>>>>>>>> entered kernel_main <<<<<<<<<\n");

  print_moss_logo();

  init_cpu();

  log_i("Hello %s", "World");
  log_i("test int: %d", 123);
  log_i("test long long: %llu", 123LL);
  log_i("test float: %f", 1.123456789);
  printf(">>>>>>>>> exited kernel_main <<<<<<<<<\n");

  post_kernel_main();
}
