#include "aarch64/oops.hh"
#include "hal/halt.hh"
#include "libcxx/log.hh"

extern_C [[noreturn]] void oops() {
  log_f("oops!!!");
  never_return();
}
