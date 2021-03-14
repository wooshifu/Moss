#include "aarch64/interrupt.hh" // for enable_interrupt
#include "aarch64/linker.hh"    // for print_memory_layout
#include "hal/init.hh"          // for init_cpu
#include "libcxx/error_code.hh" // for KErrorCode, KErrorCode::OK

KErrorCode init_cpu() {
  print_memory_layout();

  enable_interrupt();
  return KErrorCode::OK;
}
