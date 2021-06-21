#include "hal/halt.hh"          // for never_return
#include "hal/init.hh"          // for init_board, init_cpu
#include "hal/oops.hh"          // for oops
#include "kernel/board.hh"      // for print_board_name
#include "kernel/logo.hh"       // for print_moss_logo
#include "libcxx/error_code.hh" // for not_ok, KErrorCode
#include "libcxx/log.hh"        // for log_i
#include "libcxx/macro.hh"      // for extern_C
#include "libcxx/printf.hh"     // for printf

extern KErrorCode init_exception_vector_table();
extern_C [[noreturn]] void kernel_main() {
  //  KErrorCode result = pre_kernel_main();
  KErrorCode result = init_exception_vector_table();
  if (not_ok(result)) { oops("oops!!! pre kernel main error"); }

  //  result = init_board_with_hooks();
  result = init_board();
  if (not_ok(result)) { oops("oops!!! init board error"); }

  printf(">>>>>>>>> entered kernel_main <<<<<<<<<\n");

  print_moss_logo();
  print_board_name();
  int x = 0;
  log_i("addr: 0x%p", &x);

  result = init_cpu();
  if (not_ok(result)) { oops("oops!!! init cpu error"); }

  printf(">>>>>>>>> exited kernel_main <<<<<<<<<\n");

  //  post_kernel_main();
  never_return();
}
