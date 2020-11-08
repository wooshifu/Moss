#include "libc/memory.h"
#include "rpi3/mmio.h"
#include "rpi3/timer_controller.h"

void routing_core0_cntv_to_core0_irq() {
  memory_write_32bits((u32 *)CORE0_TIMER_INTERRUPT_CONTROL, COREn_CNTV_IRQ_INTERRUPT_ENABLED);
}

u32 read_core0_pending_interrupt() { return memory_read_32bits((const u32 *)CORE0_INTERRUPT_SOURCES); }
