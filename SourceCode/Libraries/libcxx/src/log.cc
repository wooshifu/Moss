#include "libcxx/log.hh"

#include "libcxx/types.hh"
#include "libhal/spin_lock.hh"
#include "libstd/stdarg.hh"
#include "libstd/stdio.hh"

void log_print(const char* fmt, ...) {
  static u64 log_print_lock{};
  auto_spin_lock lock{&log_print_lock};

  va_list ap;
  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);
}
