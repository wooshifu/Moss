//
// Created by shifu on 2021/10/17.
//

#include "libcxx/macros.hh"
#include "libcxx/types.hh"


// Hardware RNG interface.
struct hw_rng_ops {
  size_t (*hw_rng_get_entropy)(void* buf, size_t len);
};

const static struct hw_rng_ops* ops = nullptr;

size_t hw_rng_get_entropy(void* buf, size_t len) {
  if (ops != nullptr) { return ops->hw_rng_get_entropy(buf, len); }
  return 0;
}

extern_C /*__NO_SAFESTACK*/ uintptr_t choose_stack_guard(void) {
  uintptr_t guard{};
  if (hw_rng_get_entropy(&guard, sizeof(guard)) != sizeof(guard)) {
    // We can't get a random value, so use a randomish value.
    guard = 0xdeadbeef00ff00ffUL ^ (uintptr_t)&guard;
  }
  return guard;
}
