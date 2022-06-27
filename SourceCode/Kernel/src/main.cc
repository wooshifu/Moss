#include "libcxx/attr.hh"   // for attr_maybe_unused, attr_noreturn, attr_used
#include "libcxx/macros.hh" // for extern_C
#include "libcxx/types.hh"  // for paddr_t
#include "libstd/stdio.hh"  // for printf

static void print_moss() { printf("moss operating system\n"); }

extern_C attr_noreturn attr_used void kernel_main(attr_maybe_unused paddr_t handoff_paddr) {
  print_moss();

  while (true) {
    asm volatile("wfe");
  }
}
