#pragma once

#include "libcxx/types.hh"

/*
 * 暂时只4KB大小的页面和48位地址位宽
 */

constinit const auto PAGE_SHIFT  = 12;
constinit const auto TABLE_SHIFT = 9;
constinit const auto SECTION_SHIFT(PAGE_SHIFT + TABLE_SHIFT);

constinit const auto PAGE_SIZE    = (1 << PAGE_SHIFT);
constinit const auto PAGE_MASK    = (~(PAGE_SIZE - 1));
constinit const auto SECTION_SIZE = (1 << SECTION_SHIFT);

// constinit const auto LOW_MEMORY   = (2 * SECTION_SIZE);
constinit const auto TOTAL_MEMORY = (512 * 0x100000);

/* CONFIG_ARM64_VA_BITS = 48*/
constinit const auto CONFIG_ARM64_VA_BITS = 48;
constinit const auto VA_BITS              = (CONFIG_ARM64_VA_BITS);

/*
 * Memory types available.
 */
constinit const auto MT_DEVICE_nGnRnE = 0;
constinit const auto MT_DEVICE_nGnRE  = 1;
constinit const auto MT_DEVICE_GRE    = 2;
constinit const auto MT_NORMAL_NC     = 3;
constinit const auto MT_NORMAL        = 4;
constinit const auto MT_NORMAL_WT     = 5;

//#define MAIR(attr, mt) ((attr) << ((mt)*8))
consteval auto MAIR(int attr, int mt) { return ((attr) << ((mt)*8)); }

/* to align the pointer to the (next) page boundary */
//#define PAGE_ALIGN(addr)      (((addr) + PAGE_SIZE - 1) & PAGE_MASK)
constexpr auto PAGE_ALIGN(u64 addr) { return (((addr) + PAGE_SIZE - 1) & PAGE_MASK); }
//#define PAGE_ALIGN_UP(addr)   PAGE_ALIGN(addr)
consteval auto PAGE_ALIGN_UP(u64 addr) { return PAGE_ALIGN(addr); }
//#define PAGE_ALIGN_DOWN(addr) (addr & PAGE_MASK)
consteval auto PAGE_ALIGN_DOWN(u64 addr) { return (addr & PAGE_MASK); }

/* PGD */
constinit const auto PGDIR_SHIFT  = 39;
constinit const auto PGDIR_SIZE   = (1UL << PGDIR_SHIFT);
constinit const auto PGDIR_MASK   = (~(PGDIR_SIZE - 1));
constinit const auto PTRS_PER_PGD = (1 << (VA_BITS - PGDIR_SHIFT));

/* PUD */
constinit const auto PUD_SHIFT    = 30;
constinit const auto PUD_SIZE     = (1UL << PUD_SHIFT);
constinit const auto PUD_MASK     = (~(PUD_SIZE - 1));
constinit const auto PTRS_PER_PUD = (1 << (PGDIR_SHIFT - PUD_SHIFT));

/* PMD */
constinit const auto PMD_SHIFT    = 21;
constinit const auto PMD_SIZE     = (1UL << PMD_SHIFT);
constinit const auto PMD_MASK     = (~(PMD_SIZE - 1));
constinit const auto PTRS_PER_PMD = (1 << (PUD_SHIFT - PMD_SHIFT));

/* PTE */
constinit const auto PTE_SHIFT    = 12;
constinit const auto PTE_SIZE     = (1UL << PTE_SHIFT);
constinit const auto PTE_MASK     = (~(PTE_SIZE - 1));
constinit const auto PTRS_PER_PTE = (1 << (PMD_SHIFT - PTE_SHIFT));

/* Section */
// constinit const auto SECTION_SHIFT = PMD_SHIFT;
// constinit const auto SECTION_SIZE  = (1UL << SECTION_SHIFT);
constinit const auto SECTION_MASK = (~(SECTION_SIZE - 1));

/*
 * Hardware page table definitions.
 *
 * Level 1 descriptor (PUD).
 */
constinit const auto PUD_TYPE_TABLE = (3UL << 0);
constinit const auto PUD_TABLE_BIT  = (1UL << 1);
constinit const auto PUD_TYPE_MASK  = (3UL << 0);
constinit const auto PUD_TYPE_SECT  = (1UL << 0);

/*
 * Level 2 descriptor (PMD).
 */
constinit const auto PMD_TYPE_MASK  = (3UL << 0);
constinit const auto PMD_TYPE_FAULT = (0UL << 0);
constinit const auto PMD_TYPE_TABLE = (3UL << 0);
constinit const auto PMD_TYPE_SECT  = (1UL << 0);
constinit const auto PMD_TABLE_BIT  = (1UL << 1);

/*
 * Section
 */
constinit const auto PMD_SECT_VALID  = (1UL << 0);
constinit const auto PMD_SECT_USER   = (1UL << 6) /* AP[1] */;
constinit const auto PMD_SECT_RDONLY = (1UL << 7) /* AP[2] */;
constinit const auto PMD_SECT_S      = (3UL << 8);
constinit const auto PMD_SECT_AF     = (1UL << 10);
constinit const auto PMD_SECT_NG     = (1UL << 11);
constinit const auto PMD_SECT_CONT   = (1UL << 52);
constinit const auto PMD_SECT_PXN    = (1UL << 53);
constinit const auto PMD_SECT_UXN    = (1UL << 54);

/*
 * AttrIndx[2:0] encoding (mapping attributes defined in the MAIR* registers).
 */
#define PMD_ATTRINDX(t) ((t) << 2)
constinit const auto PMD_ATTRINDX_MASK = (7UL << 2);

/*
 * Level 3 descriptor (PTE).
 */
constinit const auto PTE_TYPE_MASK  = (3UL << 0);
constinit const auto PTE_TYPE_FAULT = (0UL << 0);
constinit const auto PTE_TYPE_PAGE  = (3UL << 0);
constinit const auto PTE_TABLE_BIT  = (1UL << 1);
constinit const auto PTE_USER       = (1UL << 6) /* AP[1] */;
constinit const auto PTE_RDONLY     = (1UL << 7) /* AP[2] */;
constinit const auto PTE_SHARED     = (3UL << 8) /* SH[1:0], inner shareable */;
constinit const auto PTE_AF         = (1UL << 10) /* Access Flag */;
constinit const auto PTE_NG         = (1UL << 11) /* nG */;
constinit const auto PTE_DBM        = (1UL << 51) /* Dirty Bit Management */;
constinit const auto PTE_CONT       = (1UL << 52) /* Contiguous range */;
constinit const auto PTE_PXN        = (1UL << 53) /* Privileged XN */;
constinit const auto PTE_UXN        = (1UL << 54) /* User XN */;
constinit const auto PTE_HYP_XN     = (1UL << 54) /* HYP XN */;

constinit const auto PTE_ADDR_LOW  = (((1UL << (48 - PAGE_SHIFT)) - 1) << PAGE_SHIFT);
constinit const auto PTE_ADDR_MASK = PTE_ADDR_LOW;

/*
 * AttrIndx[2:0] encoding
 * (mapping attributes defined in the MAIR* registers).
 */
//#define PTE_ATTRINDX(t) ((t) << 2)
consteval auto PTE_ATTRINDX(int t) { return t << 2; }
constinit const auto PTE_ATTRINDX_MASK = (7 << 2);

/*
 * TCR flags.
 */
constinit const auto TCR_T0SZ_OFFSET = 0;
constinit const auto TCR_T1SZ_OFFSET = 16;
//#define TCR_T0SZ(x) ((64UL - (x)) << TCR_T0SZ_OFFSET)
consteval auto TCR_T0SZ(int x) { return ((64UL - (x)) << TCR_T0SZ_OFFSET); }
//#define TCR_T1SZ(x) ((64UL - (x)) << TCR_T1SZ_OFFSET)
consteval auto TCR_T1SZ(int x) { return ((64UL - (x)) << TCR_T1SZ_OFFSET); }
//#define TCR_TxSZ(x) (TCR_T0SZ(x) | TCR_T1SZ(x))
consteval auto TCR_TxSZ(int x) { return (TCR_T0SZ(x) | TCR_T1SZ(x)); }
constinit const auto TCR_TxSZ_WIDTH = 6;
constinit const auto TCR_T0SZ_MASK  = ((((1UL) << TCR_TxSZ_WIDTH) - 1) << TCR_T0SZ_OFFSET);

constinit const auto TCR_EPD0_SHIFT  = 7;
constinit const auto TCR_EPD0_MASK   = ((1UL) << TCR_EPD0_SHIFT);
constinit const auto TCR_IRGN0_SHIFT = 8;
constinit const auto TCR_IRGN0_MASK  = ((3UL) << TCR_IRGN0_SHIFT);
constinit const auto TCR_IRGN0_NC    = ((0UL) << TCR_IRGN0_SHIFT);
constinit const auto TCR_IRGN0_WBWA  = ((1UL) << TCR_IRGN0_SHIFT);
constinit const auto TCR_IRGN0_WT    = ((2UL) << TCR_IRGN0_SHIFT);
constinit const auto TCR_IRGN0_WBnWA = ((3UL) << TCR_IRGN0_SHIFT);

constinit const auto TCR_EPD1_SHIFT  = 23;
constinit const auto TCR_EPD1_MASK   = ((1UL) << TCR_EPD1_SHIFT);
constinit const auto TCR_IRGN1_SHIFT = 24;
constinit const auto TCR_IRGN1_MASK  = ((3UL) << TCR_IRGN1_SHIFT);
constinit const auto TCR_IRGN1_NC    = ((0UL) << TCR_IRGN1_SHIFT);
constinit const auto TCR_IRGN1_WBWA  = ((1UL) << TCR_IRGN1_SHIFT);
constinit const auto TCR_IRGN1_WT    = ((2UL) << TCR_IRGN1_SHIFT);
constinit const auto TCR_IRGN1_WBnWA = ((3UL) << TCR_IRGN1_SHIFT);

constinit const auto TCR_IRGN_NC    = (TCR_IRGN0_NC | TCR_IRGN1_NC);
constinit const auto TCR_IRGN_WBWA  = (TCR_IRGN0_WBWA | TCR_IRGN1_WBWA);
constinit const auto TCR_IRGN_WT    = (TCR_IRGN0_WT | TCR_IRGN1_WT);
constinit const auto TCR_IRGN_WBnWA = (TCR_IRGN0_WBnWA | TCR_IRGN1_WBnWA);
constinit const auto TCR_IRGN_MASK  = (TCR_IRGN0_MASK | TCR_IRGN1_MASK);

constinit const auto TCR_ORGN0_SHIFT = 10;
constinit const auto TCR_ORGN0_MASK  = ((3UL) << TCR_ORGN0_SHIFT);
constinit const auto TCR_ORGN0_NC    = ((0UL) << TCR_ORGN0_SHIFT);
constinit const auto TCR_ORGN0_WBWA  = ((1UL) << TCR_ORGN0_SHIFT);
constinit const auto TCR_ORGN0_WT    = ((2UL) << TCR_ORGN0_SHIFT);
constinit const auto TCR_ORGN0_WBnWA = ((3UL) << TCR_ORGN0_SHIFT);

constinit const auto TCR_ORGN1_SHIFT = 26;
constinit const auto TCR_ORGN1_MASK  = ((3UL) << TCR_ORGN1_SHIFT);
constinit const auto TCR_ORGN1_NC    = ((0UL) << TCR_ORGN1_SHIFT);
constinit const auto TCR_ORGN1_WBWA  = ((1UL) << TCR_ORGN1_SHIFT);
constinit const auto TCR_ORGN1_WT    = ((2UL) << TCR_ORGN1_SHIFT);
constinit const auto TCR_ORGN1_WBnWA = ((3UL) << TCR_ORGN1_SHIFT);

constinit const auto TCR_ORGN_NC    = (TCR_ORGN0_NC | TCR_ORGN1_NC);
constinit const auto TCR_ORGN_WBWA  = (TCR_ORGN0_WBWA | TCR_ORGN1_WBWA);
constinit const auto TCR_ORGN_WT    = (TCR_ORGN0_WT | TCR_ORGN1_WT);
constinit const auto TCR_ORGN_WBnWA = (TCR_ORGN0_WBnWA | TCR_ORGN1_WBnWA);
constinit const auto TCR_ORGN_MASK  = (TCR_ORGN0_MASK | TCR_ORGN1_MASK);

constinit const auto TCR_SH0_SHIFT = 12;
constinit const auto TCR_SH0_MASK  = ((3UL) << TCR_SH0_SHIFT);
constinit const auto TCR_SH0_INNER = ((3UL) << TCR_SH0_SHIFT);

constinit const auto TCR_SH1_SHIFT = 28;
constinit const auto TCR_SH1_MASK  = ((3UL) << TCR_SH1_SHIFT);
constinit const auto TCR_SH1_INNER = ((3UL) << TCR_SH1_SHIFT);
constinit const auto TCR_SHARED    = (TCR_SH0_INNER | TCR_SH1_INNER);

constinit const auto TCR_TG0_SHIFT = 14;
constinit const auto TCR_TG0_MASK  = ((3UL) << TCR_TG0_SHIFT);
constinit const auto TCR_TG0_4K    = ((0UL) << TCR_TG0_SHIFT);
constinit const auto TCR_TG0_64K   = ((1UL) << TCR_TG0_SHIFT);
constinit const auto TCR_TG0_16K   = ((2UL) << TCR_TG0_SHIFT);

constinit const auto TCR_TG1_SHIFT = 30;
constinit const auto TCR_TG1_MASK  = ((3UL) << TCR_TG1_SHIFT);
constinit const auto TCR_TG1_16K   = ((1UL) << TCR_TG1_SHIFT);
constinit const auto TCR_TG1_4K    = ((2UL) << TCR_TG1_SHIFT);
constinit const auto TCR_TG1_64K   = ((3UL) << TCR_TG1_SHIFT);

constinit const auto TCR_IPS_SHIFT = 32;
constinit const auto TCR_IPS_MASK  = ((7UL) << TCR_IPS_SHIFT);
constinit const auto TCR_A1        = ((1UL) << 22);
constinit const auto TCR_ASID16    = ((1UL) << 36);
constinit const auto TCR_TBI0      = ((1UL) << 37);
constinit const auto TCR_TBI1      = ((1UL) << 38);
constinit const auto TCR_HA        = ((1UL) << 39);
constinit const auto TCR_HD        = ((1UL) << 40);
constinit const auto TCR_NFD1      = ((1UL) << 54);

constinit const auto TCR_TG_FLAGS    = (TCR_TG0_4K | TCR_TG1_4K);
constinit const auto TCR_KASLR_FLAGS = 0;
constinit const auto TCR_KASAN_FLAGS = 0;
constinit const auto TCR_SMP_FLAGS   = TCR_SHARED;
constinit const auto TCR_CACHE_FLAGS = (TCR_IRGN_WBWA | TCR_ORGN_WBWA);

/* id_aa64mmfr0 */
constinit const auto ID_AA64MMFR0_TGRAN4_SHIFT    = 28;
constinit const auto ID_AA64MMFR0_TGRAN64_SHIFT   = 24;
constinit const auto ID_AA64MMFR0_TGRAN16_SHIFT   = 20;
constinit const auto ID_AA64MMFR0_BIGENDEL0_SHIFT = 16;
constinit const auto ID_AA64MMFR0_SNSMEM_SHIFT    = 12;
constinit const auto ID_AA64MMFR0_BIGENDEL_SHIFT  = 8;
constinit const auto ID_AA64MMFR0_ASID_SHIFT      = 4;
constinit const auto ID_AA64MMFR0_PARANGE_SHIFT   = 0;

constinit const auto ID_AA64MMFR0_TGRAN4_NI         = 0xf;
constinit const auto ID_AA64MMFR0_TGRAN4_SUPPORTED  = 0x0;
constinit const auto ID_AA64MMFR0_TGRAN64_NI        = 0xf;
constinit const auto ID_AA64MMFR0_TGRAN64_SUPPORTED = 0x0;
constinit const auto ID_AA64MMFR0_TGRAN16_NI        = 0x0;
constinit const auto ID_AA64MMFR0_TGRAN16_SUPPORTED = 0x1;
constinit const auto ID_AA64MMFR0_PARANGE_48        = 0x5;
constinit const auto ID_AA64MMFR0_PARANGE_52        = 0x6;

#if defined(CONFIG_ARM64_4K_PAGES)
constinit const auto ID_AA64MMFR0_TGRAN_SHIFT     = ID_AA64MMFR0_TGRAN4_SHIFT;
constinit const auto ID_AA64MMFR0_TGRAN_SUPPORTED = ID_AA64MMFR0_TGRAN4_SUPPORTED;
#endif

/*
 * Software defined PTE bits definition.
 */
constinit const auto PTE_VALID     = (1UL << 0);
constinit const auto PTE_WRITE     = (PTE_DBM) /* same as DBM (51) */;
constinit const auto PTE_DIRTY     = (1UL << 55);
constinit const auto PTE_SPECIAL   = (1UL << 56);
constinit const auto PTE_PROT_NONE = (1UL << 58) /* only when !PTE_VALID */;

constinit const auto _PROT_DEFAULT = (PTE_TYPE_PAGE | PTE_AF | PTE_SHARED);
constinit const auto PROT_DEFAULT  = (_PROT_DEFAULT);

constinit const auto PROT_DEVICE_nGnRnE =
    (PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE | PTE_ATTRINDX(MT_DEVICE_nGnRnE));
constinit const auto PROT_DEVICE_nGnRE =
    (PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE | PTE_ATTRINDX(MT_DEVICE_nGnRE));
constinit const auto PROT_NORMAL_NC =
    (PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE | PTE_ATTRINDX(MT_NORMAL_NC));
constinit const auto PROT_NORMAL_WT =
    (PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE | PTE_ATTRINDX(MT_NORMAL_WT));
constinit const auto PROT_NORMAL = (PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE | PTE_ATTRINDX(MT_NORMAL));

constinit const auto PAGE_KERNEL = PROT_NORMAL;

constinit const auto PAGE_KERNEL_RO = ((PROT_NORMAL & ~PTE_WRITE) | PTE_RDONLY);
// constinit const auto PAGE_KERNEL_ROX=  ((PROT_NORMAL & ~(PTE_WRITE | PTE_PXN)) | PTE_RDONLY);
constinit const auto PAGE_KERNEL_EXEC = (PROT_NORMAL & ~PTE_PXN);
