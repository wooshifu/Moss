#pragma once

#include "libc/types.h"

void routing_core0_cntv_to_core0_irq();

u32 read_core0_pending_interrupt();
