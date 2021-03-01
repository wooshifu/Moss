#include "hal/interrupt.hh"   // for handle_peripheral_interrupt
#include "libcxx/memory.hh"   // for memory_write_32bits
#include "libcxx/types.hh"    // for u64, u32
#include "rpi3/mmio.hh"       // for COREn_LOCAL_TIMER_INTERRUPT_SOURCE
#include "rpi3/namespaces.hh" // for NAMESPACE_RPI3_BEGIN, NAMESPACE_RPI3_END
#include "rpi3/timer.hh"      // for handle_local_timer_irq

namespace NS_RPI3 {
  /**
   * THIS IS UNUSED INTERRUPT CONTROLLER
   *
   * see more from here:
   * https://github.com/s-matyukevich/raspberry-pi-os/blob/master/docs/lesson03/rpi-os.md#configuring-interrupt-controller
   */
  void enable_interrupt_controller() { memory_write_32bits(reinterpret_cast<u32*>(ENABLE_IRQS_1), SYSTEM_TIMER_IRQ_1); }

  bool handle_peripheral_interrupt(u64 interrupt_number) {
    if (interrupt_number & COREn_LOCAL_TIMER_INTERRUPT_SOURCE) {
      handle_local_timer_irq();
      return true;
    }
    return false;
  }

} // namespace NS_RPI3

bool handle_peripheral_interrupt(u64 interrupt_number) { return NS_RPI3::handle_peripheral_interrupt(interrupt_number); }
void enable_interrupt_controller() { NS_RPI3::enable_interrupt_controller(); }
