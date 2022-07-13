#include "libhal/spin_lock.hh"

#include "libcxx/types.hh" // for u64

void arch_spin_lock(spin_lock_t& lock) {
  u64 tmp{};
  u64 result{};
  auto* _lock = &(lock.lock);
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
               : [lock] "+r"(_lock), [tmp] "+r"(tmp), [result] "+r"(result)
               :
               : "cc", "memory");
}

void arch_spin_unlock(spin_lock_t& lock) {
  auto* _lock = &(lock.lock);
  asm volatile(R"(
    stlr    xzr, [%[lock]]
  )"
               :
               : [lock] "r"(_lock)
               : "memory");
}
