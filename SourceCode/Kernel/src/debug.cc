#include "libcxx/attr.hh"
#include "libcxx/macros.hh"
#include "libcxx/types.hh"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
size_t hw_rng_get_entropy(uintptr_t* buf, size_t len) { return 0; }
#pragma clang diagnostic pop

ATTR_NO_SAFESTACK extern_C uintptr_t choose_stack_guard() {
  uintptr_t guard = 0;
  if (hw_rng_get_entropy(&guard, sizeof(guard)) != sizeof(guard)) {
    // We can't get a random value, so use a randomish value.
    guard = 0xdeadbeef00ff00ffUL ^ (uintptr_t)&guard;
  }
  return guard;
}
