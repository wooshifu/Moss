#include "hal/el.hh"
#include "hal/init.hh"
#include "hal/stdio.hh"
#include "kernel/logo.hh"
#include "libcxx/log.hh"
#include "libcxx/macro.hh"
#include "libcxx/printf.hh"

void uart_puts(const char* s);

[[noreturn]] extern_C void kernel_main() {
  init_board();
  uart_puts("kernel main\n");
  uart_puts(">>>>>>>>> entered kernel_main <<<<<<<<<\n");

  printf("hello world %s\n", "xxx");
  printf("hello world %d\n", 1234);

//  print_moss_logo();

  uart_puts((char*)"init cpu\n");
  init_cpu();

//  uart_puts((char*)"current el: %llu", get_current_el());
  double x = 5.0;
  double y = 3.0;
  double z = x / y;
//  log_d("5/3=%f", z);

  //   unmapped virtual memory invalid access test
  //   u64 v = *(u64*)0x16;
  uart_puts(">>>>>>>>> exited kernel_main <<<<<<<<<\n");
  uart_puts("this line of code should never be executed\n");

  uart_puts("> \n");
  while (true) {
    auto ch = getchar();
    if (ch == '\r' or ch == '\n') {
      uart_puts("\n> ");
      continue;
    }
    uart_puts("got %c");
  }
}
