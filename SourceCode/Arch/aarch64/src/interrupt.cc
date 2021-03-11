#include "aarch64/interrupt.hh"
#include "aarch64/timer.hh"
#include "hal/interrupt.hh"
#include "libcxx/log.hh"
#include "libcxx/macro.hh"
#include "libcxx/types.hh"

/**
 * dispatch interrupt to cpu interrupt handler or peripheral interrupt handler
 * @param interrupt_number
 */
static void interrupt_dispatcher(u32 interrupt_number) {
  /// firstly, dispatch interrupt to cpu handler; secondly, dispatch interrupt to peripheral handler;
  bool cpu_interrupt_handled        = handle_cpu_interrupt(interrupt_number);
  bool peripheral_interrupt_handled = handle_peripheral_interrupt(interrupt_number);
  if (!cpu_interrupt_handled && !peripheral_interrupt_handled) {
    log_e("unknown pending irq: 0x%x", interrupt_number);
  }
}

extern_C void handle_interrupt(void) {
  // each core has its own pending local interrupts register
  u32 interrupt_number = read_core0_pending_interrupt();
  // log_d("irq source is: %d", interrupt_number);
  interrupt_dispatcher(interrupt_number);
}

void enable_interrupt() { asm volatile("msr daifclr, #2"); }
void disable_interrupt() { asm volatile("msr daifset, #2"); }

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
