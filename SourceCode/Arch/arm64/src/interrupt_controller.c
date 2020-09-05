#include "arch/generic_timer.h"
#include "arch/interrupt_controller.h"
#include "hal/interrupt_handler.h"
#include "libc/log.h"
#include "raspi3/timer_controller.h"

static const char *entry_error_messages[] = {
    "SYNC_INVALID_EL1t",   "IRQ_INVALID_EL1t",   "FIQ_INVALID_EL1t",   "ERROR_INVALID_EL1T",

    "SYNC_INVALID_EL1h",   "IRQ_INVALID_EL1h",   "FIQ_INVALID_EL1h",   "ERROR_INVALID_EL1h",

    "SYNC_INVALID_EL0_64", "IRQ_INVALID_EL0_64", "FIQ_INVALID_EL0_64", "ERROR_INVALID_EL0_64",

    "SYNC_INVALID_EL0_32", "IRQ_INVALID_EL0_32", "FIQ_INVALID_EL0_32", "ERROR_INVALID_EL0_32"};

void show_invalid_entry_message(int type, uint64_t esr, uint64_t address) {
  log_e("%s, ESR: %x, address: %x", entry_error_messages[type], esr, address);
}

/**
 * dispatch interrupt to cpu interrupt handler or peripheral interrupt handler
 * @param interrupt_number
 */
static void interrupt_dispatcher(uint32_t interrupt_number) {
  /// firstly, dispatch interrupt to cpu handler; secondly, dispatch interrupt to peripheral handler;
  if (handle_cpu_interrupt(interrupt_number) || handle_peripheral_interrupt(interrupt_number)) {
    return;
  }
  log_e("unknown pending irq: 0x%x", interrupt_number);
}

void handle_interrupt(void) {
  // each Core has its own pending local interrupts register
  uint32_t interrupt_number = read_core0_pending_interrupt();
  log_d("irq source is: %d", interrupt_number);
  interrupt_dispatcher(interrupt_number);
}

void enable_interrupt() { asm volatile("msr daifclr, #2"); }
void disable_interrupt() { asm volatile("msr daifset, #2"); }
