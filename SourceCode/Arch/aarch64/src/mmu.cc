#include "aarch64/mmu.h"

#include "libcxx/types.hh"

// clang-format off
[[gnu::section(".bss.prebss.translation_table")]]
[[gnu::aligned(MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP * 8)]]
[[gnu::used]]
pte_t arm64_kernel_translation_table[MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP]; // the main translation table
static_assert(MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP == 512);
// clang-format on
