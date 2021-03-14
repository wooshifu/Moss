#include "hal/init.hh"          // for init_board_with_hooks, init_cpu, pos...
#include "hal/oops.hh"          // for oops
#include "kernel/board.hh"      // for print_board_name
#include "kernel/logo.hh"       // for print_moss_logo
#include "libcxx/error_code.hh" // for not_ok, KErrorCode
#include "libcxx/macro.hh"      // for extern_C
#include "libcxx/printf.hh"     // for printf

extern_C [[noreturn]] void kernel_main() {
  pre_kernel_main();

  KErrorCode result = init_board_with_hooks();
  if (not_ok(result)) {
    oops();
  }

  printf(">>>>>>>>> entered kernel_main <<<<<<<<<\n");

  print_moss_logo();
  print_board_name();

  init_cpu();

  printf(">>>>>>>>> exited kernel_main <<<<<<<<<\n");

  post_kernel_main();
}
