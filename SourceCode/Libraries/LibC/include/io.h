#pragma once

#include <stdint.h>

void io_writeb(uint8_t val, void *addr);

void io_writew(uint16_t val, void *addr);

void io_writel(uint32_t val, void *addr);

uint8_t io_readb(void *addr);

uint16_t io_readw(void *addr);

uint32_t io_readl(void *addr);
