#include "aarch64/mp.hh"

#include "libcxx/compiler.hh" // for offsetof

// per cpu structures, each cpu will point to theirs using the fixed register
attr_used arm64_percpu arm64_percpu_array[CONF_SMP_MAX_CPUS];

static inline u32 arm64_read_percpu_u32(size_t offset) {
  u32 val; // NOLINT(cppcoreguidelines-init-variables)

  // mark as volatile to force a read of the field to make sure
  // the compiler always emits a read when asked and does not cache a copy between
  asm volatile("ldr %w[val], [x20, %[offset]]" : [val] "=r"(val) : [offset] "Ir"(offset));
  return val;
}

attr_always_inline cpu_num_t arch_curr_cpu_num() { return arm64_read_percpu_u32(offsetof(arm64_percpu, cpu_num)); }
