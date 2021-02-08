#include "hal/init.hh"     // for init_board, init_cpu
#include "kernel/logo.hh"  // for print_moss_logo
#include "libcxx/macro.hh" // for extern_C
#include "libcxx/printf.hh"

void uart_puts(const char* s);

extern_C [[gnu::noreturn]] void kernel_main() {
  init_board();

  uart_puts(">>>>>>>>> entered kernel_main <<<<<<<<<\n");

  print_moss_logo();

  init_cpu();

  printf("Hello %s\n", "World");
  printf("test int: %d\n", 123);
  printf("test long long: %llu\n", 123LL);

  //   unmapped virtual memory invalid access test
  //   u64 v = *(u64*)0x16;
  uart_puts(">>>>>>>>> exited kernel_main <<<<<<<<<\n");

  while (true) {
    asm volatile("wfi");
  }
  uart_puts("this line of code should never be executed\n");
}
