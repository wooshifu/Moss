#pragma once

#include "libcxx/types.hpp"

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
