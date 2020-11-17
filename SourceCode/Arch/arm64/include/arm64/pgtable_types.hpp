#pragma once

#include "libc/constants.hpp"
#include "libc/types.hpp"

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

#if 0
using l0_page_table_entry = u64;
static_assert(sizeof(l0_page_table_entry) == 8);
using l1_page_table_entry = u64;
using l2_page_table_entry = u64;
using l3_page_table_entry = u64;
/// 4k page
constexpr int page_table_size = 512;
using l0_page_table           = l0_page_table_entry[page_table_size];
using l1_page_table           = l1_page_table_entry[page_table_size];
using l2_page_table           = l2_page_table_entry[page_table_size];
using l3_page_table           = l3_page_table_entry[page_table_size];
static_assert(sizeof(l0_page_table) == 4_Kb);
#endif
