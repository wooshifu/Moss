#include "libcxx/macros.hh"

extern_C [[noreturn]] [[gnu::naked]] void hal_never_return() {
  asm volatile(R"(
  1:
    wfi
    b 1b
  )");
}
