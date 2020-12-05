#pragma once

#include "libcxx/types.hh"

void enable_cntv();
void disable_cntv();
u64 read_cntvct();
u32 read_cntv_tval();
void write_cntv_tval(u64 val);
u64 read_cntfrq();
void handle_generic_timer_irq();
