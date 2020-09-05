#include "libc/memory.h"
#include "raspi3/mmio.h"
#include "raspi3/timer_controller.h"

void routing_core0_cntv_to_core0_irq() {
  memory_write_32bits((uint32_t *)CORE0_TIMER_INTERRUPT_CONTROL, COREn_CNTV_IRQ_INTERRUPT_ENABLED);
}

uint32_t read_core0_pending_interrupt() { return memory_read_32bits((const uint32_t *)CORE0_INTERRUPT_SOURCES); }
