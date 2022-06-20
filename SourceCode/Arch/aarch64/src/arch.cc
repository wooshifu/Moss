#include "aarch64/mp.hh"      // for CONF_SMP_MAX_CPUS
#include "libcxx/attr.hh"     // for attr_used
#include "libcxx/compiler.hh" // for offsetof
#include "libcxx/macros.hh"   // for extern_C
#include "libcxx/types.hh"    // for uintptr_t, u64

struct arm64_sp_info_t {
  u64 mpid;
  void* sp;                  // Stack pointer points to arbitrary data.
  uintptr_t* shadow_call_sp; // SCS pointer points to array of addresses.

  // This part of the struct itself will serve temporarily as the
  // fake arch_thread in the thread pointer, so that safe-stack
  // and stack-protector code can work early.  The thread pointer
  // (TPIDR_EL1) points just past arm64_sp_info_t.
  uintptr_t stack_guard;
  void* unsafe_sp;
};

static_assert(sizeof(arm64_sp_info_t) == 40, "check arm64_get_secondary_sp assembly");
static_assert(offsetof(arm64_sp_info_t, sp) == 8, "check arm64_get_secondary_sp assembly");
static_assert(offsetof(arm64_sp_info_t, mpid) == 0, "check arm64_get_secondary_sp assembly");

// one for each CPU
attr_used arm64_sp_info_t arm64_secondary_sp_list[CONF_SMP_MAX_CPUS];

extern_C attr_used void arm64_secondary_entry() {}
