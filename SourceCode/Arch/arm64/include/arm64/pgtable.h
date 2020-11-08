#ifndef ASM_PGTABLE_H
#define ASM_PGTABLE_H

#include <arm64/pgtable_hwdef.h>
#include <arm64/pgtable_types.h>
#include <arm64/mm.h>
#include <arm64/barrier.h>
#include "arm64/mair.h"

/* 查找PGD索引 */
#define pgd_index(addr) (((addr) >> PGDIR_SHIFT) & (PTRS_PER_PGD - 1))

/* 通过地址addr查找PGD的表项 */
#define pgd_offset_raw(pgd, addr) ((pgd) + pgd_index(addr))

#define pgd_addr_end(addr, end)                                                                                        \
  ({                                                                                                                   \
    unsigned long __boundary = ((addr) + PGDIR_SIZE) & PGDIR_MASK;                                                     \
    (__boundary - 1 < (end)-1) ? __boundary : (end);                                                                   \
  })

#define pud_addr_end(addr, end)                                                                                        \
  ({                                                                                                                   \
    unsigned long __boundary = ((addr) + PUD_SIZE) & PUD_MASK;                                                         \
    (__boundary - 1 < (end)-1) ? __boundary : (end);                                                                   \
  })

#define pmd_addr_end(addr, end)                                                                                        \
  ({                                                                                                                   \
    unsigned long __boundary = ((addr) + PMD_SIZE) & PMD_MASK;                                                         \
    (__boundary - 1 < (end)-1) ? __boundary : (end);                                                                   \
  })

#define pgd_none(pgd) (!pgd_val(pgd))
#define pud_none(pud) (!pud_val(pud))
#define pmd_none(pmd) (!pmd_val(pmd))
#define ptd_none(ptd) (!ptd_val(ptd))

#define pmd_sect(pmd) ((pmd_val(pmd) & PMD_TYPE_MASK) == PMD_TYPE_SECT)
#define pud_sect(pud) ((pud_val(pud) & PUD_TYPE_MASK) == PUD_TYPE_SECT)

static inline unsigned long pgd_page_paddr(l0_page_table_entry_t pgd) { return pgd_val(pgd) & PTE_ADDR_MASK; }

#define pud_index(addr)            ((addr) >> PUD_SHIFT & (PTRS_PER_PUD - 1))
#define pud_offset_phys(pgd, addr) ((pud_t *)((pgd_page_paddr(*(pgd)) + pud_index(addr) * sizeof(pud_t))))

static inline unsigned long pud_page_paddr(pud_t pud) { return pud_val(pud) & PTE_ADDR_MASK; }

#define pmd_index(addr)            ((addr) >> PMD_SHIFT & (PTRS_PER_PMD - 1))
#define pmd_offset_phys(pud, addr) ((pmd_t *)(pud_page_paddr(*(pud)) + pmd_index(addr) * sizeof(pmd_t)))

#define pfn_pmd(pfn, prot) (__pmd(((pfn) << PMD_SHIFT) | (prot)))

static inline unsigned long pmd_page_paddr(pmd_t pmd) { return pmd_val(pmd) & PTE_ADDR_MASK; }

#define pte_index(addr)            (((addr) >> PAGE_SHIFT) & (PTRS_PER_PTE - 1))
#define pte_offset_phys(dir, addr) ((pte_t *)(pmd_page_paddr(*(dir)) + pte_index(addr) * sizeof(pte_t)))

#define pfn_pte(pfn, prot) (__pte(((pfn) << PAGE_SHIFT) | (prot)))

static inline void set_pgd(l0_page_table_entry_t *pgdp, l0_page_table_entry_t pgd) {
  *pgdp = pgd;

  dsb(ishst);
}

static inline void set_pud(pud_t *pudp, pud_t pud) {
  *pudp = pud;

  dsb(ishst);
}

static inline void set_pmd(pmd_t *pmdp, pmd_t pmd) {
  *pmdp = pmd;

  dsb(ishst);
}

static inline void set_pte(pte_t *ptep, pte_t pte) {
  *ptep = pte;
  dsb(ishst);
}

static inline unsigned long mk_sect_prot(unsigned long prot) { return prot & ~PTE_TABLE_BIT; }

typedef struct page_table_4k {
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
} __attribute__((packed)) page_table_4k;
_Static_assert(sizeof(page_table_4k) == 8, "size of page_table_4k must be 8");

const page_table_4k page_kernel_rox = {.is_valid                 = 1UL,
                                       .descriptor_type          = 1UL,
                                       .mair_attr_index          = normal_index,
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
/// NOTE: MUST be same with page_kernel_rox
#define pk_rox 0x40000000000793

// const struct page_table_4k page_kernel = (page_table_4k)0xe8000000000713;

#endif /*ASM_PGTABLE_H*/
