#include "arch/exception.h"
#include "arch/generic_timer.h"
#include "arch/interrupt_controller.h"
#include "config.h"
#include "hal/init.h"
#include "libc/log.h"

static void init_generic_timer() {
#if defined(TARGET_QEMU) && TARGET_QEMU == 1
  uint64_t counter_frequency = read_cntfrq();
  log_d("counter frq: %llu", counter_frequency);
  write_cntv_tval(counter_frequency);
  enable_cntv();
  log_d("generic timer enabled");

#elif defined(TARGET_RASPBERRY_PI3) && TARGET_RASPBERRY_PI3 == 1
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
