#include "hal/halt.hh"               // for never_return
#include "hal/init.hh"               // for init_board, init_cpu
#include "kernel/logo.hh"            // for print_moss_logo
#include "libcxx/gcc/tuple.hh"       // for get, make_tuple, tuple
#include "libcxx/gcc/type_traits.hh" // for is_array_v
#include "libcxx/log.hh"             // for log_i
#include "libcxx/macro.hh"           // for extern_C
#include "libcxx/printf.hh"          // for printf

struct C {};
extern_C [[noreturn]] void kernel_main() {
  init_board();
  std::tuple<int, double> a = std::make_tuple(1, 1.0);
  log_i("1 is: %d", std::get<int>(a));
  printf(">>>>>>>>> entered kernel_main <<<<<<<<<\n");

  print_moss_logo();

  init_cpu();

  log_i("Hello %s", "World");
  log_i("test int: %d", 123);
  log_i("test long long: %llu", 123LL);
  log_i("test float: %f", 1.123456789);
  printf(">>>>>>>>> exited kernel_main <<<<<<<<<\n");
  if (std::is_array_v<int[]>) {
    log_i("is_array_v");
  }

  never_return();
  printf("this line of code should never be executed\n");
}
