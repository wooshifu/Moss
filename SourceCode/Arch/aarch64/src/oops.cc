#include "hal/halt.hh"        // for never_return
#include "hal/oops.hh"        // for oops
#include "hal/serial_port.hh" // for is_serial_port_initialized
#include "libcxx/log.hh"      // for log_f
#include "libcxx/macro.hh"    // for extern_C

extern_C [[noreturn]] void oops() { oops("oops"); }

[[noreturn]] void oops(const char* reason) {
  [[likely]] if (is_serial_port_initialized()) {
    log_f("oops!!!");
    // todo: print sp, pc and related register values
  }
  never_return(reason);
}
