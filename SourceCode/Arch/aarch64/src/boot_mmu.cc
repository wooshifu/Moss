//
// Created by shifu on 2021/10/17.
//

#include "aarch64/config.h"
#include "libcxx/macros.hh"
#include "libcxx/types.hh"
#include "libstd/vm.h"

// Early boot time page table creation code, called from start.S while running in physical address
// space with the mmu disabled. This code should be position independent as long as it sticks to basic code.
// todo: remove #pragma
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
[[gnu::used]] extern_C int arm64_boot_map(pte_t* kernel_table0, const vaddr_t vaddr, const paddr_t paddr,
                                          const size_t len, const pte_t flags) {
  // todo: not implemented
  return 0;
}
#pragma clang diagnostic pop

[[gnu::used]] extern_C void on_boot_map_failed() {}


#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define ROUNDUP(a, b) (((a) + ((b)-1)) & ~((b)-1))
#define ROUNDDOWN(a, b) ((a) & ~((b)-1))

#define ALIGN(a, b) ROUNDUP(a, b)
#define IS_ALIGNED(a, b) (!(((uintptr_t)(a)) & (((uintptr_t)(b))-1)))


#define LOCAL_TRACE 0

/* cheezy allocator that chews up space just after the end of the kernel mapping */

/* track how much memory we've used */
extern int _end;

uintptr_t boot_alloc_start = (uintptr_t) &_end;
uintptr_t boot_alloc_end = (uintptr_t) &_end;

void *boot_alloc_mem(size_t len) {
  uintptr_t ptr = 0;

  ptr = ALIGN(boot_alloc_end, 8);
  boot_alloc_end = (ptr + ALIGN(len, 8));

//  LTRACEF("len %zu, ptr %p\n", len, (void *)ptr);

  return (void *)ptr;
}


struct mmu_initial_mapping {
  paddr_t phys;
  vaddr_t virt;
  size_t  size;
  unsigned int flags;
  const char *name;
};

#define SDRAM_BASE 0
/* Note: BCM2836/BCM2837 use different peripheral base than BCM2835 */
#define BCM_PERIPH_BASE_PHYS    (0x3f000000U)
#define BCM_PERIPH_SIZE         (0x01100000U)
#define BCM_PERIPH_BASE_VIRT    (0xffffffffc0000000ULL)
#define MEMORY_APERTURE_SIZE    (1024 * 1024 * 1024)

/* initial memory mappings. parsed by start.S */
struct mmu_initial_mapping mmu_initial_mappings[] = {
    /* 1GB of sdram space */
    {
        .phys = SDRAM_BASE,
        .virt = KERNEL_BASE,
        .size = MEMORY_APERTURE_SIZE,
        .flags = 0,
        .name = "memory"
    },

    /* peripherals */
    {
        .phys = BCM_PERIPH_BASE_PHYS,
        .virt = BCM_PERIPH_BASE_VIRT,
        .size = BCM_PERIPH_SIZE,
        .flags = MMU_INITIAL_MAPPING_FLAG_DEVICE,
        .name = "bcm peripherals"
    },

    /* null entry to terminate the list */
    { 0,0,0,0,"" }
};
