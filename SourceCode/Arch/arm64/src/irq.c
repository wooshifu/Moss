#include "irq.h"
#include "log.h"
#include "timer.h"
#include "utils.h"

const char *entry_error_messages[] = {
    "SYNC_INVALID_EL1t",   "IRQ_INVALID_EL1t",   "FIQ_INVALID_EL1t",   "ERROR_INVALID_EL1T",

    "SYNC_INVALID_EL1h",   "IRQ_INVALID_EL1h",   "FIQ_INVALID_EL1h",   "ERROR_INVALID_EL1h",

    "SYNC_INVALID_EL0_64", "IRQ_INVALID_EL0_64", "FIQ_INVALID_EL0_64", "ERROR_INVALID_EL0_64",

    "SYNC_INVALID_EL0_32", "IRQ_INVALID_EL0_32", "FIQ_INVALID_EL0_32", "ERROR_INVALID_EL0_32"};

void show_invalid_entry_message(int type, uint64_t esr, uint64_t address) {
  log_e("%s, ESR: %x, address: %x", entry_error_messages[type], esr, address);
}

void enable_interrupt_controller() { put32(ENABLE_IRQS_1, SYSTEM_TIMER_IRQ_1); }

void handle_irq(void) {
  // Each Core has its own pending local intrrupts register
  unsigned int irq = get32(CORE0_INTERRUPT_SOURCES);
  switch (irq) {
  case (LTIMER_INTERRUPT):
    handle_local_timer_irq();
    break;
  default:
    log_e("Unknown pending irq: %x\r\n", irq);
  }
}
