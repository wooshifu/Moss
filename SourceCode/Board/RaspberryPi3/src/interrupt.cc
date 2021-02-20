#include "hal/interrupt.hh"  // for handle_peripheral_interrupt
#include "libcxx/memory.hh"  // for memory_write_32bits
#include "libcxx/types.hh"   // for u32, u64
#include "rpi3/interrupt.hh" // for enable_interrupt_controller
#include "rpi3/mmio.hh"      // for COREn_LOCAL_TIMER_INTERRUPT_SOURCE, ENA...
#include "rpi3/timer.hh"     // for handle_local_timer_irq

/**
 * THIS IS UNUSED INTERRUPT CONTROLLER
 *
 * see more from here:
 * https://github.com/s-matyukevich/raspberry-pi-os/blob/master/docs/lesson03/rpi-os.md#configuring-interrupt-controller
 */
void enable_interrupt_controller() { memory_write_32bits((u32*)ENABLE_IRQS_1, SYSTEM_TIMER_IRQ_1); }

bool handle_peripheral_interrupt(u64 interrupt_number) {
  if (interrupt_number & COREn_LOCAL_TIMER_INTERRUPT_SOURCE) {
    handle_local_timer_irq();
    return true;
  }
  return false;
}
