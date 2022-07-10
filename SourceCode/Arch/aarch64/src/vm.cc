#include "libcxx/attr.hh"  // for attr_used
#include "libcxx/types.hh" // for paddr_t

// set early in arch code to record the start address of the kernel
attr_used paddr_t kernel_base_phys;
