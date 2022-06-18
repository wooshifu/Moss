#include "libcxx/attr.hh"
#include "libcxx/macros.hh"
#include "libcxx/types.hh"

attr_noreturn extern_C void kernel_main(attr_maybe_unused paddr_t handoff_paddr) {
  while (true) {
    asm("wfe");
  }
}
