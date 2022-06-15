#include "libcxx/attr.hh"
#include "libcxx/macros.hh"

attr_used attr_noreturn extern_C void kernel_main() {
  while (true) {
    asm volatile("nop");
  }

  while (true) {}
}
