#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "Simplify"
#pragma once

#include "aarch64/config.h"   // for KERNEL_ASPACE_BASE, KERNEL_BASE
#include "aarch64/defines.h"  // for PAGE_SIZE_SHIFT, SHIFT_4K, SHIFT_64K
#include "libcxx/compiler.hh" // for trigger_consteval_failure
#include "libcxx/const.hh"    // for count_bit

constexpr auto KERNEL_ASPACE_BITS = (count_bit(0xffffffffffffffff - KERNEL_ASPACE_BASE));
static_assert(KERNEL_ASPACE_BITS == 48);
constexpr auto KERNEL_BASE_BITS = (count_bit(0xffffffffffffffff - KERNEL_BASE));
static_assert(KERNEL_BASE_BITS == 48);

consteval auto MMU_KERNEL_SIZE_SHIFT() {
  if (KERNEL_ASPACE_BITS < 25) { return 25; }

  return KERNEL_ASPACE_BITS;
}
static_assert(MMU_KERNEL_SIZE_SHIFT() == 48);

constexpr auto MMU_USER_SIZE_SHIFT        = 48;
constexpr auto MMU_IDENT_SIZE_SHIFT       = 42; /* Max size supported by block mappings */

constexpr auto MMU_KERNEL_PAGE_SIZE_SHIFT = PAGE_SIZE_SHIFT();
constexpr auto MMU_USER_PAGE_SIZE_SHIFT   = USER_PAGE_SIZE_SHIFT;

consteval auto MMU_IDENT_PAGE_SIZE_SHIFT() {
  if (MMU_IDENT_SIZE_SHIFT < 25) trigger_consteval_failure("MMU_IDENT_SIZE_SHIFT too small");

  if (MMU_IDENT_SIZE_SHIFT <= 29) /* Use 2MB block mappings (4K page size) */
    return (SHIFT_4K);

  if (MMU_IDENT_SIZE_SHIFT <= 30) /* Use 512MB block mappings (64K page size) */
    return (SHIFT_64K);

  if (MMU_IDENT_SIZE_SHIFT <= 39) /* Use 1GB block mappings (4K page size) */
    return (SHIFT_4K);

  if (MMU_IDENT_SIZE_SHIFT <= 42) /* Use 512MB block mappings (64K page size) */
    return (SHIFT_64K);

  trigger_consteval_failure("MMU_IDENT_SIZE_SHIFT too large");
}

/*
 * TCR TGx values
 *
 * Page size:   4K      16K     64K
 * TG0:         0       2       1
 * TG1:         2       1       3
 */
consteval auto MMU_TG0(auto page_size_shift) {
  return (((page_size_shift == 14) & 1) << 1) | ((page_size_shift == 16) & 1);
}

consteval auto MMU_TG1(auto page_size_shift) {
  return (((page_size_shift == 12) & 1) << 1) | ((page_size_shift == 14) & 1) | ((page_size_shift == 16) & 1) |
         (((page_size_shift == 16) & 1) << 1);
}

consteval auto MMU_LX_X(auto page_shift, auto level) { return (4 - level) * (page_shift - 3) + 3; }

consteval auto MMU_USER_TOP_SHIFT() {
  if (MMU_USER_SIZE_SHIFT > MMU_LX_X(MMU_USER_PAGE_SIZE_SHIFT, 0)) return MMU_LX_X(MMU_USER_PAGE_SIZE_SHIFT, 0);

  if (MMU_USER_SIZE_SHIFT > MMU_LX_X(MMU_USER_PAGE_SIZE_SHIFT, 1)) return MMU_LX_X(MMU_USER_PAGE_SIZE_SHIFT, 1);

  if (MMU_USER_SIZE_SHIFT > MMU_LX_X(MMU_USER_PAGE_SIZE_SHIFT, 2)) return MMU_LX_X(MMU_USER_PAGE_SIZE_SHIFT, 2);

  if (MMU_USER_SIZE_SHIFT > MMU_LX_X(MMU_USER_PAGE_SIZE_SHIFT, 3)) return MMU_LX_X(MMU_USER_PAGE_SIZE_SHIFT, 3);

  trigger_consteval_failure("MMU_USER_TOP_SHIFT must produce value");
}

constexpr auto MMU_USER_PAGE_TABLE_ENTRIES_TOP = 0x1 << (MMU_USER_SIZE_SHIFT - MMU_USER_TOP_SHIFT());

consteval auto MMU_KERNEL_TOP_SHIFT() {
  if (MMU_KERNEL_SIZE_SHIFT() > MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 0)) return MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 0);

  if (MMU_KERNEL_SIZE_SHIFT() > MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 1)) return MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 1);

  if (MMU_KERNEL_SIZE_SHIFT() > MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 2)) return MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 2);

  if (MMU_KERNEL_SIZE_SHIFT() > MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 3)) return MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 3);

  trigger_consteval_failure("MMU_KERNEL_TOP_SHIFT must produce value");
}

constexpr auto MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP = (0x1 << (MMU_KERNEL_SIZE_SHIFT() - MMU_KERNEL_TOP_SHIFT()));
static_assert(MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP == 512);

consteval auto MMU_IDENT_TOP_SHIFT() {
  if (MMU_IDENT_SIZE_SHIFT > MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 0)) return MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 0);

  if (MMU_IDENT_SIZE_SHIFT > MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 1)) return MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 1);

  if (MMU_IDENT_SIZE_SHIFT > MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 2)) return MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 2);

  if (MMU_IDENT_SIZE_SHIFT > MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 3)) return MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 3);

  trigger_consteval_failure("MMU_IDENT_TOP_SHIFT must produce value");
}

constexpr auto MMU_PAGE_TABLE_ENTRIES_IDENT_SHIFT = MMU_IDENT_SIZE_SHIFT - MMU_IDENT_TOP_SHIFT();
constexpr auto MMU_PAGE_TABLE_ENTRIES_IDENT       = 0x1 << MMU_PAGE_TABLE_ENTRIES_IDENT_SHIFT;

constexpr auto MMU_PTE_DESCRIPTOR_BLOCK_MAX_SHIFT = 30;

consteval auto BM(auto base, auto count, auto val) { return (val & ((1UL << count) - 1)) << base; }

constexpr auto MMU_SH_NON_SHAREABLE              = 0;
constexpr auto MMU_SH_OUTER_SHAREABLE            = 2;
constexpr auto MMU_SH_INNER_SHAREABLE            = 3;

constexpr auto MMU_RGN_NON_CACHEABLE             = 0;
constexpr auto MMU_RGN_WRITE_BACK_ALLOCATE       = 1;
constexpr auto MMU_RGN_WRITE_THROUGH_NO_ALLOCATE = 2;
constexpr auto MMU_RGN_WRITE_BACK_NO_ALLOCATE    = 3;

constexpr auto MMU_TCR_TBI1                      = BM(38, 1, 1);
constexpr auto MMU_TCR_TBI0                      = BM(37, 1, 1);
constexpr auto MMU_TCR_AS                        = BM(36, 1, 1);
consteval auto MMU_TCR_IPS(auto size) { return BM(32, 3, size); }
consteval auto MMU_TCR_TG1(auto granule_size) { return BM(30, 2, granule_size); }
consteval auto MMU_TCR_SH1(auto shareability_flags) { return BM(28, 2, shareability_flags); }
consteval auto MMU_TCR_ORGN1(auto cache_flags) { return BM(26, 2, cache_flags); }
consteval auto MMU_TCR_IRGN1(auto cache_flags) { return BM(24, 2, cache_flags); }
constexpr auto MMU_TCR_EPD1 = BM(23, 1, 1);
constexpr auto MMU_TCR_A1   = BM(22, 1, 1);
consteval auto MMU_TCR_T1SZ(auto size) { return BM(16, 6, size); }
consteval auto MMU_TCR_TG0(auto granule_size) { return BM(14, 2, granule_size); }
consteval auto MMU_TCR_SH0(auto shareability_flags) { return BM(12, 2, shareability_flags); }
consteval auto MMU_TCR_ORGN0(auto cache_flags) { return BM(10, 2, cache_flags); }
consteval auto MMU_TCR_IRGN0(auto cache_flags) { return BM(8, 2, cache_flags); }
constexpr auto MMU_TCR_EPD0 = BM(7, 1, 1);
consteval auto MMU_TCR_T0SZ(auto size) { return BM(0, 6, size); }
consteval auto MMU_MAIR_ATTR(auto index, auto attr) { return BM(index * 8, 8, attr); }

/* L0/L1/L2/L3 descriptor types */
constexpr auto MMU_PTE_DESCRIPTOR_INVALID      = BM(0, 2, 0);
constexpr auto MMU_PTE_DESCRIPTOR_MASK         = BM(0, 2, 3);

/* L0/L1/L2 descriptor types */
constexpr auto MMU_PTE_L012_DESCRIPTOR_BLOCK   = BM(0, 2, 1);
constexpr auto MMU_PTE_L012_DESCRIPTOR_TABLE   = BM(0, 2, 3);

/* L3 descriptor types */
constexpr auto MMU_PTE_L3_DESCRIPTOR_PAGE      = BM(0, 2, 3);

/* Output address mask */
constexpr auto MMU_PTE_OUTPUT_ADDR_MASK        = BM(12, 36, 0xfffffffff);

/* Table attrs */
constexpr auto MMU_PTE_ATTR_NS_TABLE           = BM(63, 1, 1);
constexpr auto MMU_PTE_ATTR_AP_TABLE_NO_WRITE  = BM(62, 1, 1);
constexpr auto MMU_PTE_ATTR_AP_TABLE_NO_EL0    = BM(61, 1, 1);
constexpr auto MMU_PTE_ATTR_UXN_TABLE          = BM(60, 1, 1);
constexpr auto MMU_PTE_ATTR_PXN_TABLE          = BM(59, 1, 1);

/* Block/Page attrs */
constexpr auto MMU_PTE_ATTR_RES_SOFTWARE       = BM(55, 4, 0xf);
constexpr auto MMU_PTE_ATTR_UXN                = BM(54, 1, 1);
constexpr auto MMU_PTE_ATTR_PXN                = BM(53, 1, 1);
constexpr auto MMU_PTE_ATTR_CONTIGUOUS         = BM(52, 1, 1);

constexpr auto MMU_PTE_ATTR_NON_GLOBAL         = BM(11, 1, 1);
constexpr auto MMU_PTE_ATTR_AF                 = BM(10, 1, 1);

constexpr auto MMU_PTE_ATTR_SH_NON_SHAREABLE   = BM(8, 2, 0);
constexpr auto MMU_PTE_ATTR_SH_OUTER_SHAREABLE = BM(8, 2, 2);
constexpr auto MMU_PTE_ATTR_SH_INNER_SHAREABLE = BM(8, 2, 3);

constexpr auto MMU_PTE_ATTR_AP_P_RW_U_NA       = BM(6, 2, 0);
constexpr auto MMU_PTE_ATTR_AP_P_RW_U_RW       = BM(6, 2, 1);
constexpr auto MMU_PTE_ATTR_AP_P_RO_U_NA       = BM(6, 2, 2);
constexpr auto MMU_PTE_ATTR_AP_P_RO_U_RO       = BM(6, 2, 3);
constexpr auto MMU_PTE_ATTR_AP_MASK            = BM(6, 2, 3);

constexpr auto MMU_PTE_ATTR_NON_SECURE         = BM(5, 1, 1);

consteval auto MMU_PTE_ATTR_ATTR_INDEX(auto attrindex) { return BM(2, 3, attrindex); }
constexpr auto MMU_PTE_ATTR_ATTR_INDEX_MASK  = MMU_PTE_ATTR_ATTR_INDEX(7);

/* Default configuration for main kernel page table:
 *    - do cached translation walks
 */

/* Device-nGnRnE memory */
constexpr auto MMU_MAIR_ATTR0                = MMU_MAIR_ATTR(0, 0x00);
constexpr auto MMU_PTE_ATTR_STRONGLY_ORDERED = MMU_PTE_ATTR_ATTR_INDEX(0);

/* Device-nGnRE memory */
constexpr auto MMU_MAIR_ATTR1                = MMU_MAIR_ATTR(1, 0x04);
constexpr auto MMU_PTE_ATTR_DEVICE           = MMU_PTE_ATTR_ATTR_INDEX(1);

/* Normal Memory, Outer Write-back non-transient Read/Write allocate,
 * Inner Write-back non-transient Read/Write allocate
 */
constexpr auto MMU_MAIR_ATTR2                = MMU_MAIR_ATTR(2, 0xff);
constexpr auto MMU_PTE_ATTR_NORMAL_MEMORY    = MMU_PTE_ATTR_ATTR_INDEX(2);

constexpr auto MMU_MAIR_ATTR3                = (0);
constexpr auto MMU_MAIR_ATTR4                = (0);
constexpr auto MMU_MAIR_ATTR5                = (0);
constexpr auto MMU_MAIR_ATTR6                = (0);
constexpr auto MMU_MAIR_ATTR7                = (0);

constexpr auto MMU_MAIR_VAL = MMU_MAIR_ATTR0 | MMU_MAIR_ATTR1 | MMU_MAIR_ATTR2 | MMU_MAIR_ATTR3 | MMU_MAIR_ATTR4 |
                              MMU_MAIR_ATTR5 | MMU_MAIR_ATTR6 | MMU_MAIR_ATTR7;

constexpr auto MMU_TCR_IPS_DEFAULT = MMU_TCR_IPS(2); /* TODO: read at runtime, or configure per platform */

/* Enable cached page table walks:
 * inner/outer (IRGN/ORGN): write-back + write-allocate
 */
constexpr auto MMU_TCR_FLAGS1 = MMU_TCR_TG1(MMU_TG1(MMU_KERNEL_PAGE_SIZE_SHIFT)) | MMU_TCR_SH1(MMU_SH_INNER_SHAREABLE) |
                                MMU_TCR_ORGN1(MMU_RGN_WRITE_BACK_ALLOCATE) |
                                MMU_TCR_IRGN1(MMU_RGN_WRITE_BACK_ALLOCATE) | MMU_TCR_T1SZ(64 - MMU_KERNEL_SIZE_SHIFT());

constexpr auto MMU_TCR_FLAGS0 = MMU_TCR_TG0(MMU_TG0(MMU_USER_PAGE_SIZE_SHIFT)) | MMU_TCR_SH0(MMU_SH_INNER_SHAREABLE) |
                                MMU_TCR_ORGN0(MMU_RGN_WRITE_BACK_ALLOCATE) |
                                MMU_TCR_IRGN0(MMU_RGN_WRITE_BACK_ALLOCATE) | MMU_TCR_T0SZ(64 - MMU_USER_SIZE_SHIFT);
constexpr auto MMU_TCR_FLAGS0_IDENT = MMU_TCR_TG0(MMU_TG0(MMU_IDENT_PAGE_SIZE_SHIFT())) |
                                      MMU_TCR_SH0(MMU_SH_INNER_SHAREABLE) | MMU_TCR_ORGN0(MMU_RGN_WRITE_BACK_ALLOCATE) |
                                      MMU_TCR_IRGN0(MMU_RGN_WRITE_BACK_ALLOCATE) |
                                      MMU_TCR_T0SZ(64 - MMU_IDENT_SIZE_SHIFT);
constexpr auto MMU_TCR_FLAGS_IDENT  = MMU_TCR_IPS_DEFAULT | MMU_TCR_FLAGS1 | MMU_TCR_FLAGS0_IDENT;
constexpr auto MMU_TCR_FLAGS_KERNEL = MMU_TCR_IPS_DEFAULT | MMU_TCR_FLAGS1 | MMU_TCR_FLAGS0 | MMU_TCR_EPD0;
constexpr auto MMU_TCR_FLAGS_USER   = MMU_TCR_IPS_DEFAULT | MMU_TCR_FLAGS1 | MMU_TCR_FLAGS0;

consteval auto MMU_PTE_IDENT_DESCRIPTOR() {
  if (MMU_IDENT_SIZE_SHIFT > MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 2)) return MMU_PTE_L012_DESCRIPTOR_BLOCK;

  return MMU_PTE_L3_DESCRIPTOR_PAGE;
}

constexpr auto MMU_PTE_IDENT_FLAGS = MMU_PTE_IDENT_DESCRIPTOR() | MMU_PTE_ATTR_AF | MMU_PTE_ATTR_SH_INNER_SHAREABLE |
                                     MMU_PTE_ATTR_NORMAL_MEMORY | MMU_PTE_ATTR_AP_P_RW_U_NA;

constexpr auto MMU_PTE_KERNEL_RO_FLAGS = MMU_PTE_ATTR_UXN | MMU_PTE_ATTR_AF | MMU_PTE_ATTR_SH_INNER_SHAREABLE |
                                         MMU_PTE_ATTR_NORMAL_MEMORY | MMU_PTE_ATTR_AP_P_RO_U_NA;

constexpr auto MMU_PTE_KERNEL_DATA_FLAGS = MMU_PTE_ATTR_UXN | MMU_PTE_ATTR_PXN | MMU_PTE_ATTR_AF |
                                           MMU_PTE_ATTR_SH_INNER_SHAREABLE | MMU_PTE_ATTR_NORMAL_MEMORY |
                                           MMU_PTE_ATTR_AP_P_RW_U_NA;

constexpr auto MMU_INITIAL_MAP_STRONGLY_ORDERED =
    MMU_PTE_ATTR_UXN | MMU_PTE_ATTR_PXN | MMU_PTE_ATTR_AF | MMU_PTE_ATTR_STRONGLY_ORDERED | MMU_PTE_ATTR_AP_P_RW_U_NA;

constexpr auto MMU_INITIAL_MAP_DEVICE =
    MMU_PTE_ATTR_UXN | MMU_PTE_ATTR_PXN | MMU_PTE_ATTR_AF | MMU_PTE_ATTR_DEVICE | MMU_PTE_ATTR_AP_P_RW_U_NA;

#pragma clang diagnostic pop
