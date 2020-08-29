#include "hal/init.h"
#include "irq.h"

int init_cpu() {
  irq_vector_init();
  //  local_timer_init();
  enable_interrupt_controller();
  enable_irq();
  return 0;
}
