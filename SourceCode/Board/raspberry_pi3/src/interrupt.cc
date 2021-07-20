#include "hal/interrupt.hh" // for handle_peripheral_interrupt
#include "libcxx/log.hh"    // for log_d
#include "libcxx/memory.hh" // for memory_write_32bits
#include "libcxx/types.hh"  // for u64, u32
#include "rpi3/mmio.hh"     // for NS_MMIO
#include "rpi3/rpi3.hh"     // for NS_MMIO, NS_RPI3
#include "rpi3/timer.hh"    // for handle_local_timer_irq

namespace NS_rpi3 {
  /**
   * THIS IS UNUSED INTERRUPT CONTROLLER
   *
   * see more from here:
   * https://github.com/s-matyukevich/raspberry-pi-os/blob/master/docs/lesson03/rpi-os.md#configuring-interrupt-controller
   */
  void enable_interrupt_controller() {
    memory_write_32bits(reinterpret_cast<u32*>(NS_mmio::ENABLE_IRQS_1), NS_mmio::SYSTEM_TIMER_IRQ_1);
  }

  bool handle_peripheral_interrupt(u64 interrupt_number) {
    if (interrupt_number & NS_mmio::COREn_LOCAL_TIMER_INTERRUPT_SOURCE) {
      handle_local_timer_irq();
      return true;
    }
    return false;
  }

  u32 read_core0_pending_interrupt() { return memory_read_32bits((const u32*)NS_mmio::CORE0_INTERRUPT_SOURCES); }
} // namespace NS_rpi3

bool board_handle_interrupt() {
  // each core has its own pending local interrupts register
  u32 interrupt_number = NS_rpi3::read_core0_pending_interrupt();
  log_d("interrupt number: %d", interrupt_number);
  if (interrupt_number == NS_rpi3::NS_mmio::COREn_CNTV_INTERRUPT_SOURCE) {
    NS_rpi3::handle_generic_timer_irq();
    return true;
  }

  return false;
}
