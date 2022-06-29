#pragma once

#include "kernel/space.hh"

// kernel_relocated_base + 0x7000'0000
// NOTE: make sure that BCM_PERIPH_BASE_VIRT_START is not conflict with other mmu mapped memory regions
constexpr auto BCM_PERIPH_BASE_VIRT_START = 0xffff'ffff'7000'0000ULL;
constexpr auto BCM_PERIPH_BASE_PHYS       = 0x3f00'0000UL;
constexpr auto BCM_PERIPH_BASE_VIRT       = BCM_PERIPH_BASE_VIRT_START + BCM_PERIPH_BASE_PHYS;
constexpr auto MEMORY_APERTURE_SIZE       = 0x0110'0000UL;

constexpr auto MINIUART_BASE              = (BCM_PERIPH_BASE_VIRT + 0x215040);
