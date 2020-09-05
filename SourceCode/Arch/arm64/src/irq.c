#include "arch/generic_timer.h"
#include "arch/irq.h"
#include "libc/log.h"
#include "raspi3/external_timer.h"
#include "raspi3/mmio.h"
#include "raspi3/timer_controller.h"

static const char *entry_error_messages[] = {
    "SYNC_INVALID_EL1t",   "IRQ_INVALID_EL1t",   "FIQ_INVALID_EL1t",   "ERROR_INVALID_EL1T",

    "SYNC_INVALID_EL1h",   "IRQ_INVALID_EL1h",   "FIQ_INVALID_EL1h",   "ERROR_INVALID_EL1h",

    "SYNC_INVALID_EL0_64", "IRQ_INVALID_EL0_64", "FIQ_INVALID_EL0_64", "ERROR_INVALID_EL0_64",

    "SYNC_INVALID_EL0_32", "IRQ_INVALID_EL0_32", "FIQ_INVALID_EL0_32", "ERROR_INVALID_EL0_32"};

void show_invalid_entry_message(int type, uint64_t esr, uint64_t address) {
  log_e("%s, ESR: %x, address: %x", entry_error_messages[type], esr, address);
}

uint64_t count = 0;

void handle_irq(void) {
  // Each Core has its own pending local interrupts register
  //  unsigned int irq = memory_read_32bits((const uint32_t *)CORE0_INTERRUPT_SOURCES);
  unsigned int irq = read_core0_pending_interrupt();
  //  log_d("irq is: %d", irq);
  switch (irq) {
    //      case (LOCAL_TIMER_INTERRUPT):
    //        handle_local_timer_irq();
    //        break;
  case COREn_CNTV_IRQ_INTERRUPT_ENABLED:
    //    disable_irq();
    write_cntv_tval(0);
    count++;
    if (count % 100000 == 0) {
      log_d("count: %llu", count);
    }
    //    enable_irq();
    break;
  default:
    log_e("Unknown pending irq: %x\r\n", irq);
  }
}

void enable_irq() { asm volatile("msr daifclr, #2"); }
void disable_irq() { asm volatile("msr daifset, #2"); }
