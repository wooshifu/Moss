#pragma once

#include "libcxx/types.hh"

void routing_local_timer_to_core0_irq();
void handle_local_timer_irq();
void routing_core0_cntv_to_core0_irq();
u32 read_core0_pending_interrupt();
