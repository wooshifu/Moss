#include "arm64/generic_timer.h"
#include "hal/interrupt.h"

#ifndef COREn_CNTV_INTERRUPT_SOURCE
#define COREn_CNTV_INTERRUPT_SOURCE (1 << 3)
#endif

bool handle_cpu_interrupt(u64 interrupt_number) {
  if (interrupt_number & COREn_CNTV_INTERRUPT_SOURCE) {
    handle_generic_timer_irq();
    return true;
  }
  return false;
}
