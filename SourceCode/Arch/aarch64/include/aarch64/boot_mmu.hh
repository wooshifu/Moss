#include "libcxx/attr.hh"
#include "libcxx/macros.hh"
#include "libcxx/types.hh"

extern_C attr_used i64 arm64_boot_map(pte_t* kernel_table0, vaddr_t vaddr, paddr_t paddr, size_t len, pte_t flags);