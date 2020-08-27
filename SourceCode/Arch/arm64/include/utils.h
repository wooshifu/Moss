#pragma once

#include <stdint.h>

uint64_t get_current_el();

void put32(uint32_t address, uint32_t value);

uint32_t get32(uint32_t address);
