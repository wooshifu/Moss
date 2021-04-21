#if 0
#include "debug.hh"

#include "align.hh"
#include "ctype.hh"
#include "lib/crashlog.hh"
#include "platform.hh"
#include "stdio.hh"
#include "stdlib.hh"
#include "zircon/boot/crash-reason.hh"
#include "zircon/listnode.hh"
#include "zircon/time.hh"
#include "zircon/types.hh"

#include "arch/ops.hh"
#include "dev/hw_rng.hh"
#include "kernel/lockdep.hh"
#include "kernel/spinlock.hh"
#include "ktl/algorithm.hh"
#include "platform/debug.hh"

namespace {

// Start a system panic, and print a header message.
//
// Calls should be followed by:
//
//   * Calling "printf" with the reason for the panic, followed by
//     a newline.
//
//   * A call to "PanicFinish".
__ALWAYS_INLINE inline void PanicStart(void* pc, void* frame) {
  platform_panic_start();

  fprintf(&stdout_panic_buffer,
          "\n"
          "*** KERNEL PANIC (caller pc: %p, stack frame: %p):\n"
          "*** ",
          pc, frame);
}

// Finish a system panic.
//
// This function will not return, but will perform an action such as
// rebooting the system or dropping the system into a debug shell.
//
// Marked "__ALWAYS_INLINE" to avoid an additional stack frame from
// appearing in the backtrace.
__ALWAYS_INLINE __NO_RETURN inline void PanicFinish() {
  // Add a newline between the panic message and the stack trace.
  fprintf(&stdout_panic_buffer, "\n");

  platform_halt(HALT_ACTION_HALT, ZirconCrashReason::Panic);
}

// Determine if the given string ends with the given character.
bool EndsWith(const char* str, char x) {
  size_t len = strlen(str);
  return len > 0 && str[len - 1] == x;
}

}  // namespace

void spin(uint32_t usecs) {
  zx_time_t start = current_time();

  zx_duration_t nsecs = ZX_USEC(usecs);
  while (zx_time_sub_time(current_time(), start) < nsecs)
    ;
}

void panic(const char* fmt, ...) {
  PanicStart(__GET_CALLER(), __GET_FRAME());

  // Print the user message.
  va_list ap;
  va_start(ap, fmt);
  vfprintf(&stdout_panic_buffer, fmt, ap);
  va_end(ap);

  // Add a newline to the end of the panic message if it was missing.
  if (!EndsWith(fmt, '\n')) {
    fprintf(&stdout_panic_buffer, "\n");
  }

  PanicFinish();
}

void assert_fail_msg(const char* file, int line, const char* expression, const char* fmt, ...) {
  PanicStart(__GET_CALLER(), __GET_FRAME());

  // Print the user message.
  fprintf(&stdout_panic_buffer, "ASSERT FAILED at (%s:%d): %s\n", file, line, expression);
  va_list ap;
  va_start(ap, fmt);
  vfprintf(&stdout_panic_buffer, fmt, ap);
  va_end(ap);

  // Add a newline to the end of the panic message if it was missing.
  if (!EndsWith(fmt, '\n')) {
    fprintf(&stdout_panic_buffer, "\n");
  }

  PanicFinish();
}

void assert_fail(const char* file, int line, const char* expression) {
  PanicStart(__GET_CALLER(), __GET_FRAME());
  fprintf(&stdout_panic_buffer, "ASSERT FAILED at (%s:%d): %s\n", file, line, expression);
  PanicFinish();
}
#endif

#include "kernel/compiler.hh"
#include "libcxx/types.hh"

// Hardware RNG interface.
struct hw_rng_ops {
  size_t (*hw_rng_get_entropy)(void* buf, size_t len);
};

const static struct hw_rng_ops* ops = nullptr;

size_t hw_rng_get_entropy(void* buf, size_t len) {
  if (ops != nullptr) {
    return ops->hw_rng_get_entropy(buf, len);
  }     return 0;

}
#if 0
void hw_rng_register(const struct hw_rng_ops* new_ops) { ops = new_ops; }

bool hw_rng_is_registered() { return ops != nullptr; }
#endif

extern "C"
__NO_SAFESTACK uintptr_t choose_stack_guard(void) {
  uintptr_t guard{};
  if (hw_rng_get_entropy(&guard, sizeof(guard)) != sizeof(guard)) {
    // We can't get a random value, so use a randomish value.
    guard = 0xdeadbeef00ff00ffUL ^ (uintptr_t)&guard;
  }
  return guard;
}
