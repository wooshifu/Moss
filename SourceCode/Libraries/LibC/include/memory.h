#pragma once

#include <stdint.h>

void memory_write_8bits(uint8_t *address, uint8_t value);
void memory_write_16bits(uint16_t *address, uint16_t value);
void memory_write_32bits(uint32_t *address, uint32_t value);
void memory_write_64bits(uint64_t *address, uint64_t value);

uint8_t memory_read_8bits(const uint8_t *address);
uint16_t memory_read_16bits(const uint16_t *address);
uint32_t memory_read_32bits(const uint32_t *address);
uint64_t memory_read_64bits(const uint64_t *address);
