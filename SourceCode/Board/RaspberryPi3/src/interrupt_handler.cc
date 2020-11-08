#include "hal/interrupt.h"
#include "rpi3/external_timer.h"
#include "rpi3/mmio.h"

bool handle_peripheral_interrupt(u64 interrupt_number) {
  if (interrupt_number & COREn_LOCAL_TIMER_INTERRUPT_SOURCE) {
    handle_local_timer_irq();
    return true;
  }
  return false;
}
