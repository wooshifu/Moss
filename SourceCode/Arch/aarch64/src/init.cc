#include "aarch64/exception.hh" // for init_exception_vector_table
#include "aarch64/interrupt.hh" // for enable_interrupt
#include "aarch64/timer.hh"     // for enable_cntv, read_cntfrq, write_cntv...
#include "config.hh"            // for CONFIG_TARGET_QEMU
#include "hal/init.hh"          // for init_cpu
#include "libcxx/types.hh"      // for u64

static void init_generic_timer() {
#if CONFIG_TARGET_QEMU
  u64 counter_frequency = read_cntfrq();
  //  log_d("counter frq: %llu", counter_frequency);
  write_cntv_tval(counter_frequency);
  enable_cntv();
  //  log_d("generic timer enabled");

#elif CONFIG_TARGET_RASPBERRY_PI3
#error "not implemented yet";
#else
#error "unsupported";
#endif
}

int init_cpu() {
  init_exception_vector_table();

  init_generic_timer();

  enable_interrupt();
  return 0;
}
