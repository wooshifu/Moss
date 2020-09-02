#include "libc/memory.h"
#include "raspi3/interrupt_controller.h"
#include "raspi3/mmio.h"

void enable_interrupt_controller() { memory_write_32bits((uint32_t *)ENABLE_IRQS_1, SYSTEM_TIMER_IRQ_1); }
