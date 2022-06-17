#include "libcxx/attr.hh"
#include "libcxx/macros.hh"
#include "libcxx/types.hh" // for u64

u64 xxx = 0x1122334455667788;

attr_used attr_noreturn extern_C void kernel_main() {
  while (true) {
    attr_maybe_unused u64 xx = xxx;
  }

  while (true) {}
}
