#include "aarch64/mmu.hh"

#include "libcxx/attr.hh"  // for attr_used, attr_aligned
#include "libcxx/types.hh" // for paddr_t, pte_t, u64

// used at early boot and by gdb script to know the target relocated address.
attr_used u64 kernel_relocated_base        = 0xffff'ffff'0000'0000;

// The main translation table for the kernel. Globally declared because it's reached from assembly.
constexpr auto MMU_KERNEL_PAGE_TABLE_ALIGN = MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP * 8;
attr_used attr_aligned(MMU_KERNEL_PAGE_TABLE_ALIGN)
pte_t arm64_kernel_translation_table[MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP];

// Physical address of the above table, saved in start.S
attr_used paddr_t arm64_kernel_translation_table_phys;
