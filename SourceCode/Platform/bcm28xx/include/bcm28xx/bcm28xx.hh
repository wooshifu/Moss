#pragma once

#include "kernel/space.hh"

constexpr auto BCM_PERIPH_BASE_PHYS = 0x3f00'0000;
constexpr auto BCM_PERIPH_BASE_VIRT = KERNEL_SPACE_BASE + BCM_PERIPH_BASE_PHYS;
constexpr auto MEMORY_APERTURE_SIZE = 1024 * 1024 * 1024;

constexpr auto MINIUART_BASE        = (BCM_PERIPH_BASE_VIRT + 0x215040);
