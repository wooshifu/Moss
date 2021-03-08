#include "aarch64/exception.hh" // for init_exception_vector_table
#include "aarch64/interrupt.hh" // for enable_interrupt
#include "aarch64/linker.hh"
#include "aarch64/timer.hh" // for enable_cntv, read_cntfrq, write_cntv...
#include "hal/init.hh"      // for init_cpu
#include "libcxx/log.hh"
#include "libcxx/types.hh" // for u64

static void init_generic_timer() {
  u64 counter_frequency = read_cntfrq();
  log_d("counter frq: %lu", counter_frequency);
  write_cntv_tval(counter_frequency);
  enable_cntv();
  log_d("generic timer enabled");
}

int init_cpu() {
  print_memory_layout();

  init_exception_vector_table();

  init_generic_timer();

  enable_interrupt();
  return 0;
}
