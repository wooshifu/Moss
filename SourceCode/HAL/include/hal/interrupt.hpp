#pragma once

#include "libc/types.hpp"

u32 read_core0_pending_interrupt();

bool handle_cpu_interrupt(u64 interrupt_number);

bool handle_peripheral_interrupt(u64 interrupt_number);
