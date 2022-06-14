#include "libcxx/attr.hh"
#include "libcxx/types.hh"

// Static relocated base to prepare for KASLR. Used at early boot and by gdb
// script to know the target relocated address.
// TODO(fxbug.dev/24762): Choose it randomly.
#define DISABLE_KASLR 1
#if DISABLE_KASLR
u64 kernel_relocated_base = 0xffff'ffff'0000'0000;
#else
u64 kernel_relocated_base = 0xffffffff10000000;
#endif

// The main translation table for the kernel. Globally declared because it's reached from assembly.
constexpr auto MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP = 512;
ATTR_ALIGNED(MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP * 8)
ATTR_USED pte_t arm64_kernel_translation_table[MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP];

// Physical address of the above table, saved in start.S
ATTR_USED paddr_t arm64_kernel_translation_table_phys;
