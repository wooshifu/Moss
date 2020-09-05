#pragma once

#include <stdint.h>

void routing_core0_cntv_to_core0_irq();

uint32_t read_core0_pending_interrupt();
