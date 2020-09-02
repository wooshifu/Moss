#pragma once

#include <stdint.h>

void routing_core0cntv_to_core0irq();

uint32_t read_core0timer_pending();

void enable_cntv();

void disable_cntv();

uint64_t read_cntvct();

uint32_t read_cntv_tval();

void write_cntv_tval(uint64_t val);

uint64_t read_cntfrq();
