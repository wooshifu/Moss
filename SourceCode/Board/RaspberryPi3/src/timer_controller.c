#include "libc/memory.h"
#include "raspi3/timer_controller.h"

#define CORE0_TIMER_IRQCNTL 0x40000040
#define CORE0_IRQ_SOURCE 0x40000060

void routing_core0cntv_to_core0irq() { memory_write_32bits((uint32_t *)CORE0_TIMER_IRQCNTL, 0x08); }

uint32_t read_core0timer_pending() { return memory_read_32bits((const uint32_t *)CORE0_IRQ_SOURCE); }
