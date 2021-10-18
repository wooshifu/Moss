//
// Created by shifu on 2021/10/17.
//

#pragma once

constexpr auto SHIFT_4K                 = 12;
constexpr auto SHIFT_16K                = 14;
constexpr auto SHIFT_64K                = 16;

constexpr auto USER_PAGE_SIZE_SHIFT     = SHIFT_4K;
constexpr auto MMU_USER_SIZE_SHIFT      = 48;
constexpr auto MMU_USER_PAGE_SIZE_SHIFT = USER_PAGE_SIZE_SHIFT;

// For the identity map used temporarily in start.S as the cpu is trampolining
// up to the high kernel address, set the max size of the temporary address
// space constructed.
//
// Currently if between 30 and 39 the code will automatically use 4K base page
// granules, which is maximally compatible with all cores.
constexpr auto MMU_IDENT_SIZE_SHIFT     = 39;

consteval auto MMU_IDENT_PAGE_SIZE_SHIFT() {
  // Use 2MB block mappings (4K page size)
  if (MMU_IDENT_SIZE_SHIFT <= 29) { return SHIFT_4K; }
  // Use 512MB block mappings (64K page size)
  if (MMU_IDENT_SIZE_SHIFT <= 30) { return SHIFT_64K; }
  // Use 1GB block mappings (4K page size)
  if (MMU_IDENT_SIZE_SHIFT <= 39) { return SHIFT_4K; }
  // Use 512MB block mappings (64K page size)
  if (MMU_IDENT_SIZE_SHIFT <= 42) { return SHIFT_64K; }
  return 0;
}
static_assert(MMU_IDENT_PAGE_SIZE_SHIFT() != 0, "MMU_IDENT_SIZE_SHIFT too small");

// TCR TGx values
//
// Page size:   4K      16K     64K
// TG0:         0       2       1
// TG1:         2       1       3

consteval auto MMU_TG0(auto page_size_shift) {
  return (((page_size_shift == 14) & 1) << 1) | ((page_size_shift == 16) & 1);
}

consteval auto MMU_TG1(auto page_size_shift) {
  return (((page_size_shift == 12) & 1) << 1) | ((page_size_shift == 14) & 1) | ((page_size_shift == 16) & 1) |
         (((page_size_shift == 16) & 1) << 1);
}

//#define MMU_LX_X(page_shift, level) ((4 - (level)) * ((page_shift)-3) + 3)
consteval auto MMU_LX_X(auto page_shift, auto level) { return (4 - (level)) * ((page_shift)-3) + 3; }

consteval auto MMU_USER_TOP_SHIFT() {
  if (auto value = MMU_LX_X(MMU_USER_PAGE_SIZE_SHIFT, 0); MMU_USER_SIZE_SHIFT > value) { return value; }
  if (auto value = MMU_LX_X(MMU_USER_PAGE_SIZE_SHIFT, 1); MMU_USER_SIZE_SHIFT > value) { return value; }
  if (auto value = MMU_LX_X(MMU_USER_PAGE_SIZE_SHIFT, 2); MMU_USER_SIZE_SHIFT > value) { return value; }
  if (auto value = MMU_LX_X(MMU_USER_PAGE_SIZE_SHIFT, 3); MMU_USER_SIZE_SHIFT > value) { return value; }
  return 0;
}
static_assert(MMU_USER_TOP_SHIFT() != 0, "MMU_USER_TOP_SHIFT should not return 0");

consteval auto MMU_IDENT_TOP_SHIFT() {
  if (auto value = MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 0); MMU_IDENT_SIZE_SHIFT > value) { return value; }
  if (auto value = MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 1); MMU_IDENT_SIZE_SHIFT > value) { return value; }
  if (auto value = MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 2); MMU_IDENT_SIZE_SHIFT > value) { return value; }
  if (auto value = MMU_LX_X(MMU_IDENT_PAGE_SIZE_SHIFT(), 3); MMU_IDENT_SIZE_SHIFT > value) { return value; }
  return 0;
}
static_assert(MMU_IDENT_TOP_SHIFT() != 0, "MMU_IDENT_TOP_SHIFT should not return 0");

constexpr auto MMU_IDENT_PAGE_TABLE_ENTRIES_TOP_SHIFT = (MMU_IDENT_SIZE_SHIFT - MMU_IDENT_TOP_SHIFT());
static_assert(MMU_IDENT_PAGE_TABLE_ENTRIES_TOP_SHIFT == 9);
constexpr auto MMU_IDENT_PAGE_TABLE_ENTRIES_TOP = (0x1 << MMU_IDENT_PAGE_TABLE_ENTRIES_TOP_SHIFT);
static_assert(MMU_IDENT_PAGE_TABLE_ENTRIES_TOP == 512);

constexpr auto MMU_KERNEL_SIZE_SHIFT      = 48;
constexpr auto MMU_KERNEL_PAGE_SIZE_SHIFT = 12;
consteval auto MMU_KERNEL_TOP_SHIFT() {
  if (auto value = MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 0); MMU_KERNEL_SIZE_SHIFT > value) { return value; }
  if (auto value = MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 1); MMU_KERNEL_SIZE_SHIFT > value) { return value; }
  if (auto value = MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 2); MMU_KERNEL_SIZE_SHIFT > value) { return value; }
  if (auto value = MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 3); MMU_KERNEL_SIZE_SHIFT > value) { return value; }
  return 0;
}
static_assert(MMU_KERNEL_TOP_SHIFT() != 0, "MMU_KERNEL_TOP_SHIFT should not return 0");
constexpr auto MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP = 512;

// todo: add MMU_PTE_ATTR_AF | MMU_PTE_ATTR_SH_INNER_SHAREABLE | MMU_PTE_ATTR_NORMAL_MEMORY | MMU_PTE_ATTR_AP_P_RW_U_NA
//#define MMU_PTE_KERNEL_RWX_FLAGS \
//  (MMU_PTE_ATTR_AF | MMU_PTE_ATTR_SH_INNER_SHAREABLE | MMU_PTE_ATTR_NORMAL_MEMORY | MMU_PTE_ATTR_AP_P_RW_U_NA)
constexpr auto MMU_PTE_KERNEL_RWX_FLAGS          = 0x708;

consteval auto BM(auto base, auto count, auto val) { return (val & ((1UL << count) - 1)) << base; }
constexpr auto MMU_PTE_L012_DESCRIPTOR_BLOCK = BM(0, 2, 1);
constexpr auto MMU_PTE_IDENT_DESCRIPTOR      = MMU_PTE_L012_DESCRIPTOR_BLOCK;
constexpr auto MMU_PTE_IDENT_FLAGS           = (MMU_PTE_IDENT_DESCRIPTOR | MMU_PTE_KERNEL_RWX_FLAGS);

consteval auto MMU_MAIR_ATTR(auto index, auto attr) { return BM(index * 8, 8, (attr)); }
constexpr auto MMU_MAIR_ATTR0 = MMU_MAIR_ATTR(0, 0x00);

// Device-nGnRE memory
constexpr auto MMU_MAIR_ATTR1 = MMU_MAIR_ATTR(1, 0x04);
#define MMU_PTE_ATTR_DEVICE MMU_PTE_ATTR_ATTR_INDEX(1)

// Normal Memory, Outer Write-back non-transient Read/Write allocate,
// Inner Write-back non-transient Read/Write allocate
//
constexpr auto MMU_MAIR_ATTR2 = MMU_MAIR_ATTR(2, 0xff);
#define MMU_PTE_ATTR_NORMAL_MEMORY MMU_PTE_ATTR_ATTR_INDEX(2)

// Normal Memory, Inner/Outer uncached, Write Combined
constexpr auto MMU_MAIR_ATTR3 = MMU_MAIR_ATTR(3, 0x44);
#define MMU_PTE_ATTR_NORMAL_UNCACHED MMU_PTE_ATTR_ATTR_INDEX(3)

constexpr auto MMU_MAIR_ATTR4 = 0;
constexpr auto MMU_MAIR_ATTR5 = 0;
constexpr auto MMU_MAIR_ATTR6 = 0;
constexpr auto MMU_MAIR_ATTR7 = 0;
constexpr auto MMU_MAIR_VAL   = (MMU_MAIR_ATTR0 | MMU_MAIR_ATTR1 | MMU_MAIR_ATTR2 | MMU_MAIR_ATTR3 | MMU_MAIR_ATTR4 |
                               MMU_MAIR_ATTR5 | MMU_MAIR_ATTR6 | MMU_MAIR_ATTR7);

// consteval auto MMU_TCR_IPS(auto size) { return BM(32, 3, size); }
// constexpr auto MMU_TCR_AS = BM(36, 1, 1);
// constexpr auto MMU_TCR_A1 = BM(22, 1, 1);
// consteval auto MMU_TCR_TG1(auto granule_size) { return BM(30, 2, (granule_size)); }

constexpr auto MMU_TCR_TBI1   = BM(38, 1, 1);
constexpr auto MMU_TCR_TBI0   = BM(37, 1, 1);
constexpr auto MMU_TCR_AS     = BM(36, 1, 1);
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
constexpr auto MMU_TCR_EL2_RES1  = BM(31, 1, 1) | BM(23, 1, 1);
constexpr auto MMU_VTCR_EL2_RES1 = BM(31, 1, 1);
consteval auto MMU_VTCR_EL2_SL0(auto starting_level) { return BM(6, 2, (starting_level)); }

constexpr auto MMU_SH_OUTER_SHAREABLE            = 2;
constexpr auto MMU_SH_NON_SHAREABLE              = 0;
constexpr auto MMU_SH_INNER_SHAREABLE            = 3;

constexpr auto MMU_RGN_NON_CACHEABLE             = 0;
constexpr auto MMU_RGN_WRITE_BACK_ALLOCATE       = 1;
constexpr auto MMU_RGN_WRITE_THROUGH_NO_ALLOCATE = 2;
constexpr auto MMU_RGN_WRITE_BACK_NO_ALLOCATE    = 3;

// TODO: read at runtime, or configure per platform
constexpr auto MMU_TCR_IPS_DEFAULT               = MMU_TCR_IPS(2); // 40 bits
constexpr auto MMU_TCR_FLAGS1                    = (MMU_TCR_TG1(MMU_TG1(MMU_KERNEL_PAGE_SIZE_SHIFT)) |
                                 MMU_TCR_SH1(MMU_SH_INNER_SHAREABLE) | MMU_TCR_ORGN1(MMU_RGN_WRITE_BACK_ALLOCATE) |
                                 MMU_TCR_IRGN1(MMU_RGN_WRITE_BACK_ALLOCATE) | MMU_TCR_T1SZ(64 - MMU_KERNEL_SIZE_SHIFT));
constexpr auto MMU_TCR_FLAGS0 = (MMU_TCR_TG0(MMU_TG0(MMU_USER_PAGE_SIZE_SHIFT)) | MMU_TCR_SH0(MMU_SH_INNER_SHAREABLE) |
                                 MMU_TCR_ORGN0(MMU_RGN_WRITE_BACK_ALLOCATE) |
                                 MMU_TCR_IRGN0(MMU_RGN_WRITE_BACK_ALLOCATE) | MMU_TCR_T0SZ(64 - MMU_USER_SIZE_SHIFT));
constexpr auto MMU_TCR_FLAGS0_IDENT =
    (MMU_TCR_TG0(MMU_TG0(MMU_IDENT_PAGE_SIZE_SHIFT())) | MMU_TCR_SH0(MMU_SH_INNER_SHAREABLE) |
     MMU_TCR_ORGN0(MMU_RGN_WRITE_BACK_ALLOCATE) | MMU_TCR_IRGN0(MMU_RGN_WRITE_BACK_ALLOCATE) |
     MMU_TCR_T0SZ(64 - MMU_IDENT_SIZE_SHIFT));

// TCR while using the boot trampoline.
// Both TTBRs active, ASID set to kernel, ident page granule selected for user half.
constexpr auto MMU_TCR_FLAGS_IDENT =
    (MMU_TCR_IPS_DEFAULT | MMU_TCR_FLAGS1 | MMU_TCR_FLAGS0_IDENT | MMU_TCR_AS | MMU_TCR_A1);

// TCR while a kernel only (no user address space) thread is active.
// User page walks disabled, ASID set to kernel.
constexpr auto MMU_TCR_FLAGS_KERNEL =
    (MMU_TCR_IPS_DEFAULT | MMU_TCR_FLAGS1 | MMU_TCR_FLAGS0 | MMU_TCR_EPD0 | MMU_TCR_AS | MMU_TCR_A1);
