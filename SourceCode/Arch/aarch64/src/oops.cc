#include "hal/halt.hh"         // for never_return
#include "hal/oops.hh"         // for oops
#include "hal/serial_port.hh"  // for is_serial_port_initialized
#include "libcxx/log.hh"       // for log_f
#include "libcxx/macro.hh"     // for extern_C

extern_C [[noreturn]] void oops() {
  if (is_serial_port_initialized()) {
    log_f("oops!!!");
  }
  never_return();
}
