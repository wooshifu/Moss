#include "libcxx/attr.hh"
#include "libcxx/macros.hh"
#include "libcxx/types.hh"

extern_C attr_noreturn attr_used void kernel_main(attr_maybe_unused paddr_t handoff_paddr) {
  while (true) {
    asm("wfe");
  }
}
