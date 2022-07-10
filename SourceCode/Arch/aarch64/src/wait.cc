#include "libcxx/attr.hh" // for attr_noreturn

attr_noreturn void arch_wait_infinitely() {
  while (true) {
    asm volatile("wfi");
  }
}
