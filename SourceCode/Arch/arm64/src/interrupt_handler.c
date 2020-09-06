#include "arch/generic_timer.h"
#include "hal/interrupt_handler.h"
#include "raspi3/mmio.h"

bool handle_cpu_interrupt(uint64_t interrupt_number) {
  if (interrupt_number & COREn_CNTV_INTERRUPT_SOURCE) {
    handle_generic_timer_irq();
    return true;
  }
  return false;
}
