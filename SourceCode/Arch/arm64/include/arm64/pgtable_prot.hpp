#pragma once

#include "arm64/pgtable_hwdef.hpp"

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
