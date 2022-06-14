#include "libcxx/macros.hh"

[[gnu::used]] [[noreturn]] extern_C void kernel_main() {
  while (true) {
    asm volatile("nop");
  }

  while (true) {}
}
