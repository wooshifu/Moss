#include "config.h"
#include "hal/init.h"
#include "irq.h"
#include "log.h"
#include "timer.h"

static void init_timer() {
#if defined(TARGET_QEMU) && TARGET_QEMU == 1
//  local_timer_init();
//  log_d("local_timer_init");
#elif defined(TARGET_RASPBERRY_PI3) && TARGET_RASPBERRY_PI3 == 1
#warning "not implemented yet";
#else
#error "unsupported";
#endif
}

int init_cpu() {
  init_exception_vector_table();

  init_timer();

  enable_interrupt_controller();
  enable_irq();
  return 0;
}
