#include "irq.h"
#include "log.h"
#include "memory.h"
#include "timer.h"

#include "generic_timer.h"

const char *entry_error_messages[] = {
    "SYNC_INVALID_EL1t",   "IRQ_INVALID_EL1t",   "FIQ_INVALID_EL1t",   "ERROR_INVALID_EL1T",

    "SYNC_INVALID_EL1h",   "IRQ_INVALID_EL1h",   "FIQ_INVALID_EL1h",   "ERROR_INVALID_EL1h",

    "SYNC_INVALID_EL0_64", "IRQ_INVALID_EL0_64", "FIQ_INVALID_EL0_64", "ERROR_INVALID_EL0_64",

    "SYNC_INVALID_EL0_32", "IRQ_INVALID_EL0_32", "FIQ_INVALID_EL0_32", "ERROR_INVALID_EL0_32"};

void show_invalid_entry_message(int type, uint64_t esr, uint64_t address) {
  log_e("%s, ESR: %x, address: %x", entry_error_messages[type], esr, address);
}

void enable_interrupt_controller() { memory_write_32bits((uint32_t *)ENABLE_IRQS_1, SYSTEM_TIMER_IRQ_1); }

uint64_t count = 0;

void handle_irq(void) {
  // Each Core has its own pending local interrupts register
  unsigned int irq = memory_read_32bits((const uint32_t *)CORE0_INTERRUPT_SOURCES);
  //  log_d("irq is: %d", irq);
  switch (irq) {
  case (LOCAL_TIMER_INTERRUPT):
    handle_local_timer_irq();
    break;
  case 8:
    //    disable_irq();
    write_cntv_tval(0);
    count++;
    if (count % 1000000 == 0) {
      log_d("count: %llu", count);
    }
    //    enable_irq();
    break;
  default:
    log_e("Unknown pending irq: %x\r\n", irq);
  }
}
