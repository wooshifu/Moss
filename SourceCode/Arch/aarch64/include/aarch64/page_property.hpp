#pragma once

#include "libcxx/types.hpp"

/*
 * 暂时只4KB大小的页面和48位地址位宽
 */

constexpr auto PAGE_SHIFT  = 12;
constexpr auto TABLE_SHIFT = 9;
constexpr auto SECTION_SHIFT(PAGE_SHIFT + TABLE_SHIFT);

constexpr auto PAGE_SIZE    = (1 << PAGE_SHIFT);
constexpr auto PAGE_MASK    = (~(PAGE_SIZE - 1));
constexpr auto SECTION_SIZE = (1 << SECTION_SHIFT);

// constexpr auto LOW_MEMORY   = (2 * SECTION_SIZE);
constexpr auto TOTAL_MEMORY = (512 * 0x100000);

/* CONFIG_ARM64_VA_BITS = 48*/
constexpr auto CONFIG_ARM64_VA_BITS = 48;
constexpr auto VA_BITS              = (CONFIG_ARM64_VA_BITS);

/*
 * Memory types available.
 */
constexpr auto MT_DEVICE_nGnRnE = 0;
constexpr auto MT_DEVICE_nGnRE  = 1;
constexpr auto MT_DEVICE_GRE    = 2;
constexpr auto MT_NORMAL_NC     = 3;
constexpr auto MT_NORMAL        = 4;
constexpr auto MT_NORMAL_WT     = 5;

//#define MAIR(attr, mt) ((attr) << ((mt)*8))
constexpr auto MAIR(int attr, int mt) { return ((attr) << ((mt)*8)); }

/* to align the pointer to the (next) page boundary */
//#define PAGE_ALIGN(addr)      (((addr) + PAGE_SIZE - 1) & PAGE_MASK)
constexpr auto PAGE_ALIGN(u64 addr) { return (((addr) + PAGE_SIZE - 1) & PAGE_MASK); }
//#define PAGE_ALIGN_UP(addr)   PAGE_ALIGN(addr)
constexpr auto PAGE_ALIGN_UP(u64 addr) { return PAGE_ALIGN(addr); }
//#define PAGE_ALIGN_DOWN(addr) (addr & PAGE_MASK)
constexpr auto PAGE_ALIGN_DOWN(u64 addr) { return (addr & PAGE_MASK); }

/* PGD */
constexpr auto PGDIR_SHIFT  = 39;
constexpr auto PGDIR_SIZE   = (1UL << PGDIR_SHIFT);
constexpr auto PGDIR_MASK   = (~(PGDIR_SIZE - 1));
constexpr auto PTRS_PER_PGD = (1 << (VA_BITS - PGDIR_SHIFT));

/* PUD */
constexpr auto PUD_SHIFT    = 30;
constexpr auto PUD_SIZE     = (1UL << PUD_SHIFT);
constexpr auto PUD_MASK     = (~(PUD_SIZE - 1));
constexpr auto PTRS_PER_PUD = (1 << (PGDIR_SHIFT - PUD_SHIFT));

/* PMD */
constexpr auto PMD_SHIFT    = 21;
constexpr auto PMD_SIZE     = (1UL << PMD_SHIFT);
constexpr auto PMD_MASK     = (~(PMD_SIZE - 1));
constexpr auto PTRS_PER_PMD = (1 << (PUD_SHIFT - PMD_SHIFT));

/* PTE */
constexpr auto PTE_SHIFT    = 12;
constexpr auto PTE_SIZE     = (1UL << PTE_SHIFT);
constexpr auto PTE_MASK     = (~(PTE_SIZE - 1));
constexpr auto PTRS_PER_PTE = (1 << (PMD_SHIFT - PTE_SHIFT));

/* Section */
// constexpr auto SECTION_SHIFT = PMD_SHIFT;
// constexpr auto SECTION_SIZE  = (1UL << SECTION_SHIFT);
constexpr auto SECTION_MASK = (~(SECTION_SIZE - 1));

/*
 * Hardware page table definitions.
 *
 * Level 1 descriptor (PUD).
 */
constexpr auto PUD_TYPE_TABLE = (3UL << 0);
constexpr auto PUD_TABLE_BIT  = (1UL << 1);
constexpr auto PUD_TYPE_MASK  = (3UL << 0);
constexpr auto PUD_TYPE_SECT  = (1UL << 0);

/*
 * Level 2 descriptor (PMD).
 */
constexpr auto PMD_TYPE_MASK  = (3UL << 0);
constexpr auto PMD_TYPE_FAULT = (0UL << 0);
constexpr auto PMD_TYPE_TABLE = (3UL << 0);
constexpr auto PMD_TYPE_SECT  = (1UL << 0);
constexpr auto PMD_TABLE_BIT  = (1UL << 1);

/*
 * Section
 */
constexpr auto PMD_SECT_VALID  = (1UL << 0);
constexpr auto PMD_SECT_USER   = (1UL << 6) /* AP[1] */;
constexpr auto PMD_SECT_RDONLY = (1UL << 7) /* AP[2] */;
constexpr auto PMD_SECT_S      = (3UL << 8);
constexpr auto PMD_SECT_AF     = (1UL << 10);
constexpr auto PMD_SECT_NG     = (1UL << 11);
constexpr auto PMD_SECT_CONT   = (1UL << 52);
constexpr auto PMD_SECT_PXN    = (1UL << 53);
constexpr auto PMD_SECT_UXN    = (1UL << 54);

/*
 * AttrIndx[2:0] encoding (mapping attributes defined in the MAIR* registers).
 */
#define PMD_ATTRINDX(t) ((t) << 2)
constexpr auto PMD_ATTRINDX_MASK = (7UL << 2);

/*
 * Level 3 descriptor (PTE).
 */
constexpr auto PTE_TYPE_MASK  = (3UL << 0);
constexpr auto PTE_TYPE_FAULT = (0UL << 0);
constexpr auto PTE_TYPE_PAGE  = (3UL << 0);
constexpr auto PTE_TABLE_BIT  = (1UL << 1);
constexpr auto PTE_USER       = (1UL << 6) /* AP[1] */;
constexpr auto PTE_RDONLY     = (1UL << 7) /* AP[2] */;
constexpr auto PTE_SHARED     = (3UL << 8) /* SH[1:0], inner shareable */;
constexpr auto PTE_AF         = (1UL << 10) /* Access Flag */;
constexpr auto PTE_NG         = (1UL << 11) /* nG */;
constexpr auto PTE_DBM        = (1UL << 51) /* Dirty Bit Management */;
constexpr auto PTE_CONT       = (1UL << 52) /* Contiguous range */;
constexpr auto PTE_PXN        = (1UL << 53) /* Privileged XN */;
constexpr auto PTE_UXN        = (1UL << 54) /* User XN */;
constexpr auto PTE_HYP_XN     = (1UL << 54) /* HYP XN */;

constexpr auto PTE_ADDR_LOW  = (((1UL << (48 - PAGE_SHIFT)) - 1) << PAGE_SHIFT);
constexpr auto PTE_ADDR_MASK = PTE_ADDR_LOW;

/*
 * AttrIndx[2:0] encoding
 * (mapping attributes defined in the MAIR* registers).
 */
//#define PTE_ATTRINDX(t) ((t) << 2)
constexpr auto PTE_ATTRINDX(int t) { return t << 2; }
constexpr auto PTE_ATTRINDX_MASK = (7 << 2);

/*
 * TCR flags.
 */
constexpr auto TCR_T0SZ_OFFSET = 0;
constexpr auto TCR_T1SZ_OFFSET = 16;
//#define TCR_T0SZ(x) ((64UL - (x)) << TCR_T0SZ_OFFSET)
constexpr auto TCR_T0SZ(int x) { return ((64UL - (x)) << TCR_T0SZ_OFFSET); }
//#define TCR_T1SZ(x) ((64UL - (x)) << TCR_T1SZ_OFFSET)
constexpr auto TCR_T1SZ(int x) { return ((64UL - (x)) << TCR_T1SZ_OFFSET); }
//#define TCR_TxSZ(x) (TCR_T0SZ(x) | TCR_T1SZ(x))
constexpr auto TCR_TxSZ(int x) { return (TCR_T0SZ(x) | TCR_T1SZ(x)); }
constexpr auto TCR_TxSZ_WIDTH = 6;
constexpr auto TCR_T0SZ_MASK  = ((((1UL) << TCR_TxSZ_WIDTH) - 1) << TCR_T0SZ_OFFSET);

constexpr auto TCR_EPD0_SHIFT  = 7;
constexpr auto TCR_EPD0_MASK   = ((1UL) << TCR_EPD0_SHIFT);
constexpr auto TCR_IRGN0_SHIFT = 8;
constexpr auto TCR_IRGN0_MASK  = ((3UL) << TCR_IRGN0_SHIFT);
constexpr auto TCR_IRGN0_NC    = ((0UL) << TCR_IRGN0_SHIFT);
constexpr auto TCR_IRGN0_WBWA  = ((1UL) << TCR_IRGN0_SHIFT);
constexpr auto TCR_IRGN0_WT    = ((2UL) << TCR_IRGN0_SHIFT);
constexpr auto TCR_IRGN0_WBnWA = ((3UL) << TCR_IRGN0_SHIFT);

constexpr auto TCR_EPD1_SHIFT  = 23;
constexpr auto TCR_EPD1_MASK   = ((1UL) << TCR_EPD1_SHIFT);
constexpr auto TCR_IRGN1_SHIFT = 24;
constexpr auto TCR_IRGN1_MASK  = ((3UL) << TCR_IRGN1_SHIFT);
constexpr auto TCR_IRGN1_NC    = ((0UL) << TCR_IRGN1_SHIFT);
constexpr auto TCR_IRGN1_WBWA  = ((1UL) << TCR_IRGN1_SHIFT);
constexpr auto TCR_IRGN1_WT    = ((2UL) << TCR_IRGN1_SHIFT);
constexpr auto TCR_IRGN1_WBnWA = ((3UL) << TCR_IRGN1_SHIFT);

constexpr auto TCR_IRGN_NC    = (TCR_IRGN0_NC | TCR_IRGN1_NC);
constexpr auto TCR_IRGN_WBWA  = (TCR_IRGN0_WBWA | TCR_IRGN1_WBWA);
constexpr auto TCR_IRGN_WT    = (TCR_IRGN0_WT | TCR_IRGN1_WT);
constexpr auto TCR_IRGN_WBnWA = (TCR_IRGN0_WBnWA | TCR_IRGN1_WBnWA);
constexpr auto TCR_IRGN_MASK  = (TCR_IRGN0_MASK | TCR_IRGN1_MASK);

constexpr auto TCR_ORGN0_SHIFT = 10;
constexpr auto TCR_ORGN0_MASK  = ((3UL) << TCR_ORGN0_SHIFT);
constexpr auto TCR_ORGN0_NC    = ((0UL) << TCR_ORGN0_SHIFT);
constexpr auto TCR_ORGN0_WBWA  = ((1UL) << TCR_ORGN0_SHIFT);
constexpr auto TCR_ORGN0_WT    = ((2UL) << TCR_ORGN0_SHIFT);
constexpr auto TCR_ORGN0_WBnWA = ((3UL) << TCR_ORGN0_SHIFT);

constexpr auto TCR_ORGN1_SHIFT = 26;
constexpr auto TCR_ORGN1_MASK  = ((3UL) << TCR_ORGN1_SHIFT);
constexpr auto TCR_ORGN1_NC    = ((0UL) << TCR_ORGN1_SHIFT);
constexpr auto TCR_ORGN1_WBWA  = ((1UL) << TCR_ORGN1_SHIFT);
constexpr auto TCR_ORGN1_WT    = ((2UL) << TCR_ORGN1_SHIFT);
constexpr auto TCR_ORGN1_WBnWA = ((3UL) << TCR_ORGN1_SHIFT);

constexpr auto TCR_ORGN_NC    = (TCR_ORGN0_NC | TCR_ORGN1_NC);
constexpr auto TCR_ORGN_WBWA  = (TCR_ORGN0_WBWA | TCR_ORGN1_WBWA);
constexpr auto TCR_ORGN_WT    = (TCR_ORGN0_WT | TCR_ORGN1_WT);
constexpr auto TCR_ORGN_WBnWA = (TCR_ORGN0_WBnWA | TCR_ORGN1_WBnWA);
constexpr auto TCR_ORGN_MASK  = (TCR_ORGN0_MASK | TCR_ORGN1_MASK);

constexpr auto TCR_SH0_SHIFT = 12;
constexpr auto TCR_SH0_MASK  = ((3UL) << TCR_SH0_SHIFT);
constexpr auto TCR_SH0_INNER = ((3UL) << TCR_SH0_SHIFT);

constexpr auto TCR_SH1_SHIFT = 28;
constexpr auto TCR_SH1_MASK  = ((3UL) << TCR_SH1_SHIFT);
constexpr auto TCR_SH1_INNER = ((3UL) << TCR_SH1_SHIFT);
constexpr auto TCR_SHARED    = (TCR_SH0_INNER | TCR_SH1_INNER);

constexpr auto TCR_TG0_SHIFT = 14;
constexpr auto TCR_TG0_MASK  = ((3UL) << TCR_TG0_SHIFT);
constexpr auto TCR_TG0_4K    = ((0UL) << TCR_TG0_SHIFT);
constexpr auto TCR_TG0_64K   = ((1UL) << TCR_TG0_SHIFT);
constexpr auto TCR_TG0_16K   = ((2UL) << TCR_TG0_SHIFT);

constexpr auto TCR_TG1_SHIFT = 30;
constexpr auto TCR_TG1_MASK  = ((3UL) << TCR_TG1_SHIFT);
constexpr auto TCR_TG1_16K   = ((1UL) << TCR_TG1_SHIFT);
constexpr auto TCR_TG1_4K    = ((2UL) << TCR_TG1_SHIFT);
constexpr auto TCR_TG1_64K   = ((3UL) << TCR_TG1_SHIFT);

constexpr auto TCR_IPS_SHIFT = 32;
constexpr auto TCR_IPS_MASK  = ((7UL) << TCR_IPS_SHIFT);
constexpr auto TCR_A1        = ((1UL) << 22);
constexpr auto TCR_ASID16    = ((1UL) << 36);
constexpr auto TCR_TBI0      = ((1UL) << 37);
constexpr auto TCR_TBI1      = ((1UL) << 38);
constexpr auto TCR_HA        = ((1UL) << 39);
constexpr auto TCR_HD        = ((1UL) << 40);
constexpr auto TCR_NFD1      = ((1UL) << 54);

constexpr auto TCR_TG_FLAGS    = (TCR_TG0_4K | TCR_TG1_4K);
constexpr auto TCR_KASLR_FLAGS = 0;
constexpr auto TCR_KASAN_FLAGS = 0;
constexpr auto TCR_SMP_FLAGS   = TCR_SHARED;
constexpr auto TCR_CACHE_FLAGS = (TCR_IRGN_WBWA | TCR_ORGN_WBWA);

/* id_aa64mmfr0 */
constexpr auto ID_AA64MMFR0_TGRAN4_SHIFT    = 28;
constexpr auto ID_AA64MMFR0_TGRAN64_SHIFT   = 24;
constexpr auto ID_AA64MMFR0_TGRAN16_SHIFT   = 20;
constexpr auto ID_AA64MMFR0_BIGENDEL0_SHIFT = 16;
constexpr auto ID_AA64MMFR0_SNSMEM_SHIFT    = 12;
constexpr auto ID_AA64MMFR0_BIGENDEL_SHIFT  = 8;
constexpr auto ID_AA64MMFR0_ASID_SHIFT      = 4;
constexpr auto ID_AA64MMFR0_PARANGE_SHIFT   = 0;

constexpr auto ID_AA64MMFR0_TGRAN4_NI         = 0xf;
constexpr auto ID_AA64MMFR0_TGRAN4_SUPPORTED  = 0x0;
constexpr auto ID_AA64MMFR0_TGRAN64_NI        = 0xf;
constexpr auto ID_AA64MMFR0_TGRAN64_SUPPORTED = 0x0;
constexpr auto ID_AA64MMFR0_TGRAN16_NI        = 0x0;
constexpr auto ID_AA64MMFR0_TGRAN16_SUPPORTED = 0x1;
constexpr auto ID_AA64MMFR0_PARANGE_48        = 0x5;
constexpr auto ID_AA64MMFR0_PARANGE_52        = 0x6;

#if defined(CONFIG_ARM64_4K_PAGES)
constexpr auto ID_AA64MMFR0_TGRAN_SHIFT     = ID_AA64MMFR0_TGRAN4_SHIFT;
constexpr auto ID_AA64MMFR0_TGRAN_SUPPORTED = ID_AA64MMFR0_TGRAN4_SUPPORTED;
#endif

/*
 * Software defined PTE bits definition.
 */
constexpr auto PTE_VALID     = (1UL << 0);
constexpr auto PTE_WRITE     = (PTE_DBM) /* same as DBM (51) */;
constexpr auto PTE_DIRTY     = (1UL << 55);
constexpr auto PTE_SPECIAL   = (1UL << 56);
constexpr auto PTE_PROT_NONE = (1UL << 58) /* only when !PTE_VALID */;

constexpr auto _PROT_DEFAULT = (PTE_TYPE_PAGE | PTE_AF | PTE_SHARED);
constexpr auto PROT_DEFAULT  = (_PROT_DEFAULT);

constexpr auto PROT_DEVICE_nGnRnE =
    (PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE | PTE_ATTRINDX(MT_DEVICE_nGnRnE));
constexpr auto PROT_DEVICE_nGnRE =
    (PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE | PTE_ATTRINDX(MT_DEVICE_nGnRE));
constexpr auto PROT_NORMAL_NC = (PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE | PTE_ATTRINDX(MT_NORMAL_NC));
constexpr auto PROT_NORMAL_WT = (PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE | PTE_ATTRINDX(MT_NORMAL_WT));
constexpr auto PROT_NORMAL    = (PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE | PTE_ATTRINDX(MT_NORMAL));

constexpr auto PAGE_KERNEL = PROT_NORMAL;

constexpr auto PAGE_KERNEL_RO = ((PROT_NORMAL & ~PTE_WRITE) | PTE_RDONLY);
// constexpr auto PAGE_KERNEL_ROX=  ((PROT_NORMAL & ~(PTE_WRITE | PTE_PXN)) | PTE_RDONLY);
constexpr auto PAGE_KERNEL_EXEC = (PROT_NORMAL & ~PTE_PXN);
