#include "hal/init.hh"           // for init_board_with_hooks, init_cpu, pos...
#include "hal/init_hook.hh"
#include "hal/oops.hh"           // for oops
#include "kernel/board.hh"       // for print_board_name
#include "kernel/logo.hh"        // for print_moss_logo
#include "libcxx/error_code.hh"  // for not_ok, KErrorCode
#include "libcxx/macro.hh"       // for extern_C
#include "libcxx/printf.hh"      // for printf

extern_C [[noreturn]] void kernel_main() {
  KErrorCode result = pre_kernel_main();
  if (not_ok(result)) { oops("oops!!! pre kernel main error"); }

  result = init_board_with_hooks();
  if (not_ok(result)) { oops("oops!!! init board error"); }

  printf(">>>>>>>>> entered kernel_main <<<<<<<<<\n");

  print_moss_logo();
  print_board_name();

  result = init_cpu();
  if (not_ok(result)) { oops("oops!!! init cpu error"); }

  printf(">>>>>>>>> exited kernel_main <<<<<<<<<\n");

  post_kernel_main();
}
