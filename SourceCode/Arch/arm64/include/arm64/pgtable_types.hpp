#pragma once

#include "libcxx/constants.hpp"
#include "libcxx/types.hpp"

/*
typedef u64 pteval_t;
typedef u64 pmdval_t;
typedef u64 pudval_t;
typedef u64 pgdval_t;

typedef struct {
  pteval_t pte;
} pte_t;
#define pte_val(x) ((x).pte)
#define __pte(x)   ((pte_t){(x)})

typedef struct {
  pmdval_t pmd;
} pmd_t;
#define pmd_val(x) ((x).pmd)
#define __pmd(x)   ((pmd_t){(x)})

typedef struct {
  pudval_t pud;
} pud_t;
#define pud_val(x) ((x).pud)
#define __pud(x)   ((pud_t){(x)})

typedef struct {
  pgdval_t pgd;
} l0_page_table_entry_t;
#define pgd_val(x) ((x).pgd)
#define __pgd(x)   ((l0_page_table_entry_t){(x)})
*/

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
static_assert(sizeof(L0PageTable) == PAGE_SIZE, "l0_page_table must be size of 4096");
static_assert(sizeof(L1PageTable) == PAGE_SIZE, "l1_page_table must be size of 4096");
static_assert(sizeof(L2PageTable) == PAGE_SIZE, "l2_page_table must be size of 4096");
static_assert(sizeof(L3PageTable) == PAGE_SIZE, "l3_page_table must be size of 4096");
