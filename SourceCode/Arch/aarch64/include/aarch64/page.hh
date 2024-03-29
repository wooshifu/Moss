#pragma once

#include "aarch64/barrier.hh"
#include "aarch64/mair.hh"
#include "aarch64/page_property.hh"

constexpr unsigned long make_section_property(const unsigned long property) { return property & ~PTE_TABLE_BIT; }

using page_table_4k = struct page_table_4k {
  u64 is_valid                                : 1;  /// bit 0
  u64 descriptor_type                         : 1;  /// bit 1
  enum mair_field_value_index mair_attr_index : 3;  /// bit [2:4]
  u64 non_secure                              : 1;  /// bit [5]
  u64 data_access_permission                  : 2;  /// bit [6:7]
  u64 shareability                            : 2;  /// bit [8:9]
  u64 access_flag                             : 1;  /// bit 10
  u64 not_global                              : 1;  /// bit 11
  u64 output_address                          : 36; /// bit [12:47]
  u64 reserved0                               : 4;  /// bit [48:51]
  u64 contiguous                              : 1;  /// bit 52
  u64 privileged_execute_never                : 1;  /// bit 53
  u64 execute_never                           : 1;  /// bit 54
  u64 ignored                                 : 9;  /// bit [55:63]
};
static_assert(sizeof(page_table_4k) == 8, "size of page_table_4k must be 8");

consteval u64 page_table_4k_to_u64(const page_table_4k page_table) {
  return page_table.is_valid << 0 | page_table.descriptor_type << 1 | page_table.mair_attr_index << 2 |
         page_table.non_secure << 5 | page_table.data_access_permission << 6 | page_table.shareability << 8 |
         page_table.access_flag << 10 | page_table.not_global << 11 | page_table.output_address << 12 |
         (u64)(page_table.reserved0) << 48 | (u64)(page_table.contiguous) << 52 |
         (u64)(page_table.privileged_execute_never) << 53 | (u64)(page_table.execute_never) << 54 |
         (u64)(page_table.ignored) << 55;
}

constexpr page_table_4k _page_kernel_rox = {.is_valid                 = 1UL,
                                            .descriptor_type          = 1UL,
                                            .mair_attr_index          = mair_field_value_index::normal_index,
                                            .non_secure               = 0UL,
                                            .data_access_permission   = 0b10UL,
                                            .shareability             = 0b11UL,
                                            .access_flag              = 1UL,
                                            .not_global               = 0UL,
                                            .output_address           = 0UL,
                                            .reserved0                = 0UL,
                                            .contiguous               = 0UL,
                                            .privileged_execute_never = 0UL,
                                            .execute_never            = 1UL,
                                            .ignored                  = 0UL};
constexpr u64 PAGE_KERNEL_ROX            = page_table_4k_to_u64(_page_kernel_rox);
static_assert(PAGE_KERNEL_ROX == 0x40000000000793, "page_kernel_rox must be 0x40000000000793");
// const struct page_table_4k page_kernel = (page_table_4k)0xe8000000000713;

using PageTableEntryType = u64;
static_assert(sizeof(PageTableEntryType) == 8);
using L0PageTableEntry = PageTableEntryType;
using L1PageTableEntry = PageTableEntryType;
using L2PageTableEntry = PageTableEntryType;
using L3PageTableEntry = PageTableEntryType;

/// 4k page
constexpr int PAGE_TABLE_SIZE = PAGE_SIZE / sizeof(PageTableEntryType);
static_assert(PAGE_TABLE_SIZE == 512);
using L0PageTable = L0PageTableEntry[PAGE_TABLE_SIZE];
using L1PageTable = L1PageTableEntry[PAGE_TABLE_SIZE];
using L2PageTable = L2PageTableEntry[PAGE_TABLE_SIZE];
using L3PageTable = L3PageTableEntry[PAGE_TABLE_SIZE];
static_assert(sizeof(L0PageTable) == PAGE_SIZE, "L0PageTable must be size of 4096");
static_assert(sizeof(L1PageTable) == PAGE_SIZE, "L1PageTable must be size of 4096");
static_assert(sizeof(L2PageTable) == PAGE_SIZE, "L2PageTable must be size of 4096");
static_assert(sizeof(L3PageTable) == PAGE_SIZE, "L3PageTable must be size of 4096");
