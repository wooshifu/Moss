#include "libcxx/log.hh"

#include "libcxx/types.hh"     // for u64
#include "libhal/spin_lock.hh" // for auto_spin_lock
#include "libstd/stdarg.hh"    // for va_end, va_list, va_start
#include "libstd/stdio.hh"     // for vprintf

void log_print(const char* fmt, ...) {
  static u64 log_print_lock{};
  auto_spin_lock lock{&log_print_lock};

  va_list ap;
  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);
}
