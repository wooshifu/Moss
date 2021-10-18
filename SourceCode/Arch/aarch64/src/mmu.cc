//
// Created by shifu on 2021/10/17.
//

#include "aarch64/mmu.hh"
#include "libcxx/types.hh"
#include "libcxx/unit.hh"

// The main translation table for the kernel. Globally declared because it's reached from assembly.
// constexpr auto MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP = 512;
constexpr auto page_table_size = MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP * sizeof(u64);
static_assert(page_table_size == 4_KiB);
[[gnu::used]] [[gnu::aligned(page_table_size)]] u64 arm64_kernel_translation_table[MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP];
