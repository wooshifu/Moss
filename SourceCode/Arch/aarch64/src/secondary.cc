//
// Created by shifu on 2021/10/17.
//

#include "kconfig.hh"
#include "libcxx/compiler.hh"
#include "libcxx/macros.hh"
#include "libcxx/types.hh"

[[gnu::naked]] extern_C u64 arm64_get_secondary_sp() {
  asm volatile(R"asm_code(
.include "aarch64/asm_macros.hh"

  mrs     x9, mpidr_el1
  ldr     x10, =0xff00ffffff  /* Mask for AFFx (cluster) ids */
  and     x9, x9, x10
  mov     x10, %[_CONFIG_SMP_MAX_CPUS]

  adr_global x11, arm64_secondary_sp_list

  .Lsp_loop:
  ldr     x12, [x11, #0]
  cmp     x12, x9
  beq     .Lsp_found
  add     x11, x11, #40
  subs    x10, x10, #1
  bne     .Lsp_loop
  mov     x0, xzr
  mov     x1, xzr
  mov     x2, xzr
  ret

  .Lsp_found:
  ldr     x0, [x11, #8]
  ldr     x2, [x11, #16]
  add     x1, x11, #40
  ret
)asm_code"
               :
               : [_CONFIG_SMP_MAX_CPUS] "i"(CONFIG_SMP_MAX_CPUS)
               :);
}

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
arm64_sp_info_t arm64_secondary_sp_list[CONFIG_SMP_MAX_CPUS];

// called from assembly.
// todo: not implemented
extern "C" [[noreturn]] void arm64_secondary_entry() {
  while (true) {
    asm volatile(R"asm_code(
      wfi
    )asm_code");
  }
}
