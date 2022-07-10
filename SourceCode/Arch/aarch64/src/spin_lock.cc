#include "libcxx/types.hh"

void arch_spin_lock(void* lock) {
  u64 tmp{};
  u64 result{};
  asm volatile(R"(
    mov     %[tmp], #1
    sevl
1:
    wfe
    ldaxr   %[result], [%[lock]]
    cbnz    %[result], 1b
    stxr    %w[result], %[tmp], [%[lock]]
    cbnz    %w[result], 1b
  )"
               : [lock] "+r"(lock), [tmp] "+r"(tmp), [result] "+r"(result)
               :
               : "cc", "memory");
}

void arch_spin_unlock(void* lock) {
  asm volatile(R"(
    stlr    xzr, [%[lock]]
  )"
               :
               : [lock] "r"(lock)
               : "memory");
}
