#include "libcxx/attr.hh"   // for attr_noreturn, attr_used
#include "libcxx/log.hh"    // for log_i
#include "libcxx/macros.hh" // for extern_C
#include "libhal/cpu.hh"    // for arch_curr_cpu_num
#include "libhal/wait.hh"   // for wait_infinitely

extern_C attr_noreturn attr_used void secondary_entry() {
  log_i("cpu_id: %d", arch_curr_cpu_num());

  wait_infinitely();
}
