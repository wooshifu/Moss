#pragma once

#include "libcxx/types.hpp"

constexpr auto PAGE_SHIFT  = 12;
constexpr auto TABLE_SHIFT = 9;
constexpr auto SECTION_SHIFT(PAGE_SHIFT + TABLE_SHIFT);

constexpr auto PAGE_SIZE    = (1 << PAGE_SHIFT);
constexpr auto PAGE_MASK    = (~(PAGE_SIZE - 1));
constexpr auto SECTION_SIZE = (1 << SECTION_SHIFT);

// constexpr auto LOW_MEMORY   = (2 * SECTION_SIZE);
constexpr auto TOTAL_MEMORY = (512 * 0x100000);

/* CONFIG_ARM64_VA_BITS = 48*/
constexpr auto CONFIG_ARM64_VA_BITS = 48;
constexpr auto VA_BITS              = (CONFIG_ARM64_VA_BITS);

/*
 * Memory types available.
 */
constexpr auto MT_DEVICE_nGnRnE = 0;
constexpr auto MT_DEVICE_nGnRE  = 1;
constexpr auto MT_DEVICE_GRE    = 2;
constexpr auto MT_NORMAL_NC     = 3;
constexpr auto MT_NORMAL        = 4;
constexpr auto MT_NORMAL_WT     = 5;

//#define MAIR(attr, mt) ((attr) << ((mt)*8))
constexpr auto MAIR(int attr, int mt) { return ((attr) << ((mt)*8)); }

/* to align the pointer to the (next) page boundary */
//#define PAGE_ALIGN(addr)      (((addr) + PAGE_SIZE - 1) & PAGE_MASK)
constexpr auto PAGE_ALIGN(u64 addr) { return (((addr) + PAGE_SIZE - 1) & PAGE_MASK); }
//#define PAGE_ALIGN_UP(addr)   PAGE_ALIGN(addr)
constexpr auto PAGE_ALIGN_UP(u64 addr) { return PAGE_ALIGN(addr); }
//#define PAGE_ALIGN_DOWN(addr) (addr & PAGE_MASK)
constexpr auto PAGE_ALIGN_DOWN(u64 addr) { return (addr & PAGE_MASK); }
