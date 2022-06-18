#include "libcxx/attr.hh"
#include "libcxx/types.hh"

// used at early boot and by gdb script to know the target relocated address.
attr_used u64 kernel_relocated_base              = 0xffff'ffff'0000'0000;

// The main translation table for the kernel. Globally declared because it's reached from assembly.
constexpr auto MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP = 512;
constexpr auto MMU_KERNEL_PAGE_TABLE_ALIGN       = MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP * 8;
attr_used attr_aligned(MMU_KERNEL_PAGE_TABLE_ALIGN)
pte_t arm64_kernel_translation_table[MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP];

// Physical address of the above table, saved in start.S
attr_used paddr_t arm64_kernel_translation_table_phys;
