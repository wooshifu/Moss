#include "hal/interrupt_handler.h"
#include "raspi3/external_timer.h"
#include "raspi3/mmio.h"

bool handle_peripheral_interrupt(uint64_t interrupt_number) {
  if (interrupt_number & COREn_LOCAL_TIMER_INTERRUPT_SOURCE) {
    handle_local_timer_irq();
    return true;
  }
  return false;
}
