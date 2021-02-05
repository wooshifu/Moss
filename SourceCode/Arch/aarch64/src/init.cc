#include "aarch64/exception.hh"
#include "aarch64/interrupt.hh"
#include "aarch64/timer.hh"
#include "config.hh"
#include "hal/init.hh"
#include "libcxx/log.hh"
#include "libcxx/types.hh"

static void init_generic_timer() {
#if CONFIG_TARGET_QEMU
  u64 counter_frequency = read_cntfrq();
  log_d("counter frq: %llu", counter_frequency);
  write_cntv_tval(counter_frequency);
  enable_cntv();
  log_d("generic timer enabled");

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
