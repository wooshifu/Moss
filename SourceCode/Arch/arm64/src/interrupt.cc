#include "arm64/asm.hpp"
#include "arm64/interrupt.hpp"
#include "arm64/timer.hpp"
#include "hal/interrupt.hpp"
#include "libc/log.hpp"

static const char *const entry_error_messages[] = {
    "SYNC_INVALID_EL1t",   "IRQ_INVALID_EL1t",   "FIQ_INVALID_EL1t",   "ERROR_INVALID_EL1T",

    "SYNC_INVALID_EL1h",   "IRQ_INVALID_EL1h",   "FIQ_INVALID_EL1h",   "ERROR_INVALID_EL1h",

    "SYNC_INVALID_EL0_64", "IRQ_INVALID_EL0_64", "FIQ_INVALID_EL0_64", "ERROR_INVALID_EL0_64",

    "SYNC_INVALID_EL0_32", "IRQ_INVALID_EL0_32", "FIQ_INVALID_EL0_32", "ERROR_INVALID_EL0_32"};

EXTERN_C void show_invalid_entry_message(int type, u64 esr, u64 address) {
  log_e("%s, ESR: %x, address: %x", entry_error_messages[type], esr, address);
}

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

EXTERN_C void handle_interrupt(void) {
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
