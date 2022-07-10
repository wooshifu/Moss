#include "libcxx/attr.hh"   // for attr_maybe_unused, attr_noreturn, attr_used
#include "libcxx/log.hh"    // for log_print, log_i
#include "libcxx/macros.hh" // for extern_C
#include "libcxx/types.hh"  // for paddr_t
#include "libhal/cpu.hh"    // for arch_curr_cpu_num
#include "libhal/wait.hh"   // for wait_infinitely

static void print_moss() { log_print("moss operating system\n"); }

extern_C attr_noreturn attr_used void kernel_main(attr_maybe_unused paddr_t handoff_paddr) {
  print_moss();

  log_i("cpu_id: %d", arch_curr_cpu_num());

  wait_infinitely();
}
