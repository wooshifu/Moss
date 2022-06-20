#include "aarch64/boot_alloc.hh"

#include "aarch64/mmu.hh"  // for PAGE_SIZE
#include "kernel/aligh.hh" // for ALIGN
#include "libcxx/attr.hh"  // for attr_no_safestack
#include "libcxx/types.hh" // for paddr_t

// store the start and current pointer to the boot allocator in physical address
attr_used paddr_t boot_alloc_start;
attr_used paddr_t boot_alloc_end;

// called from arch start.S
// run in physical space without the mmu set up, so stick to basic, relocatable code
paddr_t boot_alloc_page_phys() {
  paddr_t ptr    = ALIGN(boot_alloc_end, PAGE_SIZE);
  boot_alloc_end = ptr + PAGE_SIZE;

  return ptr;
}
