#pragma once

#include "kernel/space.hh"

consteval auto BM(auto base, auto count, auto val) { return (val & ((1UL << count) - 1)) << base; }

// Block/Page attrs
constexpr auto MMU_PTE_ATTR_RES_SOFTWARE       = BM(55, 4, 0xf);
constexpr auto MMU_PTE_ATTR_XN                 = BM(54, 1, 1); // for single translation regimes
constexpr auto MMU_PTE_ATTR_UXN                = BM(54, 1, 1); // 1<<54
constexpr auto MMU_PTE_ATTR_PXN                = BM(53, 1, 1); // 1<<53
constexpr auto MMU_PTE_ATTR_CONTIGUOUS         = BM(52, 1, 1); // 1<<52

constexpr auto MMU_PTE_ATTR_NON_GLOBAL         = BM(11, 1, 1); // 1<<11
constexpr auto MMU_PTE_ATTR_AF                 = BM(10, 1, 1); // 1<<10

constexpr auto MMU_PTE_ATTR_SH_NON_SHAREABLE   = BM(8, 2, 0); // 0
constexpr auto MMU_PTE_ATTR_SH_OUTER_SHAREABLE = BM(8, 2, 2); // 2<<8
constexpr auto MMU_PTE_ATTR_SH_INNER_SHAREABLE = BM(8, 2, 3); // 3<<8

constexpr auto MMU_PTE_ATTR_AP_P_RW_U_NA       = BM(6, 2, 0); // 0
constexpr auto MMU_PTE_ATTR_AP_P_RW_U_RW       = BM(6, 2, 1); // 1<<6
constexpr auto MMU_PTE_ATTR_AP_P_RO_U_NA       = BM(6, 2, 2); // 2<<6
constexpr auto MMU_PTE_ATTR_AP_P_RO_U_RO       = BM(6, 2, 3); // 3<<6
constexpr auto MMU_PTE_ATTR_AP_MASK            = BM(6, 2, 3); // 3<<6

constexpr auto MMU_PTE_ATTR_NON_SECURE         = BM(5, 1, 1);

consteval auto MMU_PTE_ATTR_ATTR_INDEX(auto attrindex) { return BM(2, 3, attrindex); }
constexpr auto MMU_PTE_ATTR_ATTR_INDEX_MASK = MMU_PTE_ATTR_ATTR_INDEX(7);

constexpr auto MMU_PTE_PERMISSION_MASK      = (MMU_PTE_ATTR_AP_MASK | MMU_PTE_ATTR_UXN | MMU_PTE_ATTR_PXN);

constexpr auto MMU_PTE_ATTR_NORMAL_MEMORY   = MMU_PTE_ATTR_ATTR_INDEX(2);
static_assert(MMU_PTE_ATTR_NORMAL_MEMORY == 8);

constexpr auto MMU_PTE_KERNEL_RWX_FLAGS = (MMU_PTE_ATTR_UXN | MMU_PTE_ATTR_AF | MMU_PTE_ATTR_SH_INNER_SHAREABLE |
                                           MMU_PTE_ATTR_NORMAL_MEMORY | MMU_PTE_ATTR_AP_P_RW_U_NA);
static_assert(MMU_PTE_KERNEL_RWX_FLAGS == ((1ULL << 54) | (1 << 10) | (3 << 8) | (8) | (0)));
static_assert(MMU_PTE_KERNEL_RWX_FLAGS == 0x40000000000708);

constexpr auto MMU_PTE_KERNEL_RO_FLAGS =
    (MMU_PTE_ATTR_UXN | MMU_PTE_ATTR_PXN | MMU_PTE_ATTR_AF | MMU_PTE_ATTR_SH_INNER_SHAREABLE |
     MMU_PTE_ATTR_NORMAL_MEMORY | MMU_PTE_ATTR_AP_P_RO_U_NA);

constexpr auto MMU_PTE_KERNEL_DATA_FLAGS =
    (MMU_PTE_ATTR_UXN | MMU_PTE_ATTR_PXN | MMU_PTE_ATTR_AF | MMU_PTE_ATTR_SH_INNER_SHAREABLE |
     MMU_PTE_ATTR_NORMAL_MEMORY | MMU_PTE_ATTR_AP_P_RW_U_NA);
static_assert(MMU_PTE_KERNEL_DATA_FLAGS == ((1ULL << 54) | (1ULL << 53) | (1 << 10) | (3 << 8) | (8) | (0)));
static_assert(MMU_PTE_KERNEL_DATA_FLAGS == 0x60000000000708);

consteval auto IFTE(auto c, auto t, auto e) { return ((!!c) * t | !c * e); }
consteval auto NBITS01(auto n) { return IFTE(n, 1, 0); }
consteval auto NBITS02(auto n) { return IFTE(n >> 1, 1 + NBITS01(n >> 1), NBITS01(n)); }
consteval auto NBITS04(auto n) { return IFTE(n >> 2, 2 + NBITS02(n >> 2), NBITS02(n)); }
consteval auto NBITS08(auto n) { return IFTE(n >> 4, 4 + NBITS04(n >> 4), NBITS04(n)); }
consteval auto NBITS16(auto n) { return IFTE(n >> 8, 8 + NBITS08(n >> 8), NBITS08(n)); }
consteval auto NBITS32(auto n) { return IFTE(n >> 16, 16 + NBITS16(n >> 16), NBITS16(n)); }
consteval auto NBITS(auto n) { return IFTE(n >> 32, 32 + NBITS32(n >> 32), NBITS32(n)); }

constexpr auto KERNEL_ASPACE_BITS = NBITS(0xffffffffffffffff - KERNEL_SPACE_BASE);
static_assert(KERNEL_ASPACE_BITS == 48);

constexpr auto MMU_KERNEL_SIZE_SHIFT = KERNEL_ASPACE_BITS;
constexpr auto MMU_USER_SIZE_SHIFT   = 48;

constexpr auto SHIFT_4K              = 12;
constexpr auto SHIFT_16K             = 14;
constexpr auto SHIFT_64K             = 16;

constexpr auto PAGE_SIZE_SHIFT       = SHIFT_4K;
constexpr auto PAGE_SIZE             = 1L << PAGE_SIZE_SHIFT;
static_assert(PAGE_SIZE == 4096);

constexpr auto MMU_KERNEL_PAGE_SIZE_SHIFT = PAGE_SIZE_SHIFT;
constexpr auto USER_PAGE_SIZE_SHIFT       = SHIFT_4K;
constexpr auto MMU_USER_PAGE_SIZE_SHIFT   = USER_PAGE_SIZE_SHIFT;

consteval auto MMU_LX_X(auto page_shift, auto level) { return (4 - level) * (page_shift - 3) + 3; }

consteval auto MMU_KERNEL_TOP_SHIFT() {
  if (MMU_KERNEL_SIZE_SHIFT > MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 0)) return MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 0);

  if (MMU_KERNEL_SIZE_SHIFT > MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 1)) return MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 1);

  if (MMU_KERNEL_SIZE_SHIFT > MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 2)) return MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 2);

  if (MMU_KERNEL_SIZE_SHIFT > MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 3)) return MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 3);
}

constexpr auto MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP = (0x1 << (MMU_KERNEL_SIZE_SHIFT - MMU_KERNEL_TOP_SHIFT()));
static_assert(MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP == 512);
constexpr auto MMU_KERNEL_PAGE_TABLE_ENTRIES = (0x1 << (MMU_KERNEL_PAGE_SIZE_SHIFT - 3));
static_assert(MMU_KERNEL_PAGE_TABLE_ENTRIES == 512);
//------------------------------------------------

// For the identity map used temporarily in start.S as the cpu is trampolining
// up to the high kernel address, set the max size of the temporary address
// space constructed.
//
// Currently if between 30 and 39 the code will automatically use 4K base page
// granules, which is maximally compatible with all cores.
constexpr auto MMU_IDENT_SIZE_SHIFT = 39;

consteval auto MMU_IDENT_PAGE_SIZE_SHIFT() {
  static_assert(!(MMU_IDENT_SIZE_SHIFT < 25));

  if (MMU_IDENT_SIZE_SHIFT <= 29) // Use 2MB block mappings (4K page size)
    return (SHIFT_4K);

  if (MMU_IDENT_SIZE_SHIFT <= 30) // Use 512MB block mappings (64K page size)
    return (SHIFT_64K);

  if (MMU_IDENT_SIZE_SHIFT <= 39) // Use 1GB block mappings (4K page size)
    return (SHIFT_4K);

  if (MMU_IDENT_SIZE_SHIFT <= 42) // Use 512MB block mappings (64K page size)
    return (SHIFT_64K);
}

consteval auto _MMU_IDENT_TOP_SHIFT() {
  if (MMU_IDENT_SIZE_SHIFT > MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 0)) return MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 0);

  if (MMU_IDENT_SIZE_SHIFT > MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 1)) return MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 1);

  if (MMU_IDENT_SIZE_SHIFT > MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 2)) return MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 2);

  if (MMU_IDENT_SIZE_SHIFT > MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 3)) return MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 3);
}
constexpr auto MMU_IDENT_TOP_SHIFT = _MMU_IDENT_TOP_SHIFT();
static_assert(MMU_IDENT_TOP_SHIFT == 30);

constexpr auto MMU_IDENT_PAGE_TABLE_ENTRIES_TOP_SHIFT = (MMU_IDENT_SIZE_SHIFT - MMU_IDENT_TOP_SHIFT);
static_assert(MMU_IDENT_PAGE_TABLE_ENTRIES_TOP_SHIFT == 9);
constexpr auto MMU_IDENT_PAGE_TABLE_ENTRIES_TOP = 0x1 << MMU_IDENT_PAGE_TABLE_ENTRIES_TOP_SHIFT;
static_assert(MMU_IDENT_PAGE_TABLE_ENTRIES_TOP == 512);
//------------------------------

// L0/L1/L2/L3 descriptor types
constexpr auto MMU_PTE_DESCRIPTOR_INVALID    = BM(0, 2, 0);
constexpr auto MMU_PTE_DESCRIPTOR_MASK       = BM(0, 2, 3);

// L0/L1/L2 descriptor types
constexpr auto MMU_PTE_L012_DESCRIPTOR_BLOCK = BM(0, 2, 1);
constexpr auto MMU_PTE_L012_DESCRIPTOR_TABLE = BM(0, 2, 3);

// L3 descriptor types
constexpr auto MMU_PTE_L3_DESCRIPTOR_PAGE    = BM(0, 2, 3);

// Output address mask
constexpr auto MMU_PTE_OUTPUT_ADDR_MASK      = BM(12, 36, 0xfffffffff);

consteval auto MMU_PTE_IDENT_DESCRIPTOR() {
  if (MMU_IDENT_SIZE_SHIFT > MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 2)) return MMU_PTE_L012_DESCRIPTOR_BLOCK;
  return MMU_PTE_L3_DESCRIPTOR_PAGE;
}
constexpr auto MMU_PTE_IDENT_FLAGS = (MMU_PTE_IDENT_DESCRIPTOR() | MMU_PTE_KERNEL_RWX_FLAGS);
static_assert(MMU_PTE_IDENT_FLAGS == (1 | 0x40000000000708));
static_assert(MMU_PTE_IDENT_FLAGS == 0x40000000000709);

//------------
consteval auto MMU_MAIR_ATTR(auto index, auto attr) { return BM(index * 8, 8, attr); }
// Device-nGnRnE memory
constexpr auto MMU_MAIR_ATTR0 = MMU_MAIR_ATTR(0, 0x00);
static_assert(MMU_MAIR_ATTR0 == 0);
constexpr auto MMU_PTE_ATTR_STRONGLY_ORDERED = MMU_PTE_ATTR_ATTR_INDEX(0);

// Device-nGnRE memory
constexpr auto MMU_MAIR_ATTR1                = MMU_MAIR_ATTR(1, 0x04);
static_assert(MMU_MAIR_ATTR1 == 0x0400);
constexpr auto MMU_PTE_ATTR_DEVICE = MMU_PTE_ATTR_ATTR_INDEX(1);

// Normal Memory, Outer Write-back non-transient Read/Write allocate,
// Inner Write-back non-transient Read/Write allocate
//
constexpr auto MMU_MAIR_ATTR2      = MMU_MAIR_ATTR(2, 0xff);
static_assert(MMU_MAIR_ATTR2 == 0xff0000);
// constexpr auto MMU_PTE_ATTR_NORMAL_MEMORY    = MMU_PTE_ATTR_ATTR_INDEX(2);

// Normal Memory, Inner/Outer uncached, Write Combined
constexpr auto MMU_MAIR_ATTR3 = MMU_MAIR_ATTR(3, 0x44);
static_assert(MMU_MAIR_ATTR3 == 0x44000000);
constexpr auto MMU_PTE_ATTR_NORMAL_UNCACHED = MMU_PTE_ATTR_ATTR_INDEX(3);

constexpr auto MMU_MAIR_ATTR4               = 0;
constexpr auto MMU_MAIR_ATTR5               = 0;
constexpr auto MMU_MAIR_ATTR6               = 0;
constexpr auto MMU_MAIR_ATTR7               = 0;

constexpr auto MMU_MAIR_VAL = MMU_MAIR_ATTR0 | MMU_MAIR_ATTR1 | MMU_MAIR_ATTR2 | MMU_MAIR_ATTR3 | MMU_MAIR_ATTR4 |
                              MMU_MAIR_ATTR5 | MMU_MAIR_ATTR6 | MMU_MAIR_ATTR7;
static_assert(MMU_MAIR_VAL == 0x44ff0400);

//------------

constexpr auto MMU_TCR_TBI1 = BM(38, 1, 1);
constexpr auto MMU_TCR_TBI0 = BM(37, 1, 1);
constexpr auto MMU_TCR_AS   = BM(36, 1, 1);
consteval auto MMU_TCR_IPS(auto size) { return BM(32, 3, (size)); }
consteval auto MMU_TCR_TG1(auto granule_size) { return BM(30, 2, (granule_size)); }
consteval auto MMU_TCR_SH1(auto shareability_flags) { return BM(28, 2, (shareability_flags)); }
consteval auto MMU_TCR_ORGN1(auto cache_flags) { return BM(26, 2, (cache_flags)); }
consteval auto MMU_TCR_IRGN1(auto cache_flags) { return BM(24, 2, (cache_flags)); }
constexpr auto MMU_TCR_EPD1 = BM(23, 1, 1);
constexpr auto MMU_TCR_A1   = BM(22, 1, 1);
consteval auto MMU_TCR_T1SZ(auto size) { return BM(16, 6, (size)); }
consteval auto MMU_TCR_TG0(auto granule_size) { return BM(14, 2, (granule_size)); }
consteval auto MMU_TCR_SH0(auto shareability_flags) { return BM(12, 2, (shareability_flags)); }
consteval auto MMU_TCR_ORGN0(auto cache_flags) { return BM(10, 2, (cache_flags)); }
consteval auto MMU_TCR_IRGN0(auto cache_flags) { return BM(8, 2, (cache_flags)); }
constexpr auto MMU_TCR_EPD0 = BM(7, 1, 1);
consteval auto MMU_TCR_T0SZ(auto size) { return BM(0, 6, (size)); }

constexpr auto MMU_TCR_EL2_RES1                  = (BM(31, 1, 1) | BM(23, 1, 1));

// TODO: read at runtime, or configure per platform
constexpr auto MMU_TCR_IPS_DEFAULT               = MMU_TCR_IPS(2); // 40 bits;

constexpr auto MMU_SH_NON_SHAREABLE              = 0;
constexpr auto MMU_SH_OUTER_SHAREABLE            = 2;
constexpr auto MMU_SH_INNER_SHAREABLE            = 3;

constexpr auto MMU_RGN_NON_CACHEABLE             = 0;
constexpr auto MMU_RGN_WRITE_BACK_ALLOCATE       = 1;
constexpr auto MMU_RGN_WRITE_THROUGH_NO_ALLOCATE = 2;
constexpr auto MMU_RGN_WRITE_BACK_NO_ALLOCATE    = 3;

// TCR TGx values
//
// Page size:   4K      16K     64K
// TG0:         0       2       1
// TG1:         2       1       3

consteval auto MMU_TG0(auto page_size_shift) {
  return ((((page_size_shift == 14) & 1) << 1) | ((page_size_shift == 16) & 1));
}

consteval auto MMU_TG1(auto page_size_shift) {
  return ((((page_size_shift == 12) & 1) << 1) | ((page_size_shift == 14) & 1) | ((page_size_shift == 16) & 1) |
          (((page_size_shift == 16) & 1) << 1));
}

// Enable cached page table walks:
// inner/outer (IRGN/ORGN): write-back + write-allocate
constexpr auto MMU_TCR_FLAGS1 = (MMU_TCR_TG1(MMU_TG1(MMU_KERNEL_PAGE_SIZE_SHIFT)) |
                                 MMU_TCR_SH1(MMU_SH_INNER_SHAREABLE) | MMU_TCR_ORGN1(MMU_RGN_WRITE_BACK_ALLOCATE) |
                                 MMU_TCR_IRGN1(MMU_RGN_WRITE_BACK_ALLOCATE) | MMU_TCR_T1SZ(64 - MMU_KERNEL_SIZE_SHIFT));
constexpr auto MMU_TCR_FLAGS0 = (MMU_TCR_TG0(MMU_TG0(MMU_USER_PAGE_SIZE_SHIFT)) | MMU_TCR_SH0(MMU_SH_INNER_SHAREABLE) |
                                 MMU_TCR_ORGN0(MMU_RGN_WRITE_BACK_ALLOCATE) |
                                 MMU_TCR_IRGN0(MMU_RGN_WRITE_BACK_ALLOCATE) | MMU_TCR_T0SZ(64 - MMU_USER_SIZE_SHIFT));
constexpr auto MMU_TCR_FLAGS0_IDENT =
    (MMU_TCR_TG0(MMU_TG0(MMU_IDENT_PAGE_SIZE_SHIFT())) | MMU_TCR_SH0(MMU_SH_INNER_SHAREABLE) |
     MMU_TCR_ORGN0(MMU_RGN_WRITE_BACK_ALLOCATE) | MMU_TCR_IRGN0(MMU_RGN_WRITE_BACK_ALLOCATE) |
     MMU_TCR_T0SZ(64 - MMU_IDENT_SIZE_SHIFT));

// TCR while using the boot trampoline. Both TTBRs active, ASID set to kernel, ident page granule selected for user half
constexpr auto MMU_TCR_FLAGS_IDENT =
    (MMU_TCR_IPS_DEFAULT | MMU_TCR_FLAGS1 | MMU_TCR_FLAGS0_IDENT | MMU_TCR_AS | MMU_TCR_A1);
static_assert(MMU_TCR_FLAGS_IDENT == 0x12'b550'3519);

// TCR while a kernel only (no user address space) thread is active. User page walks disabled, ASID set to kernel
constexpr auto MMU_TCR_FLAGS_KERNEL =
    (MMU_TCR_IPS_DEFAULT | MMU_TCR_FLAGS1 | MMU_TCR_FLAGS0 | MMU_TCR_EPD0 | MMU_TCR_AS | MMU_TCR_A1);
static_assert(MMU_TCR_FLAGS_KERNEL == 0x12'b550'3590);
// TCR while a user mode thread is active in user or kernel space. Both TTBrs active, ASID set to user
constexpr auto MMU_TCR_FLAGS_USER = (MMU_TCR_IPS_DEFAULT | MMU_TCR_FLAGS1 | MMU_TCR_FLAGS0 | MMU_TCR_TBI0 | MMU_TCR_AS);
