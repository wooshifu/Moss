#pragma once

#include <stdbool.h>
#include <stdint.h>

bool handle_cpu_interrupt(uint64_t interrupt_number);

bool handle_peripheral_interrupt(uint64_t interrupt_number);
