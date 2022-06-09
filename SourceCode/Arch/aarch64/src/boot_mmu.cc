#include "aarch64/config.h"
#include "libcxx/macros.hh"
#include "libcxx/types.hh"
#include "libcxx/unit.hh"
#include "libstd/vm.h"

constexpr auto ROUNDUP(auto a, auto b) { return (a + (b - 1)) & ~(b - 1); }

constexpr auto ALIGN(auto a, auto b) { return ROUNDUP(a, b); }

/* track how much memory we've used */
extern int _end;

[[gnu::used]] uintptr_t boot_alloc_start = (uintptr_t)&_end; // NOLINT(cppcoreguidelines-interfaces-global-init)
[[gnu::used]] uintptr_t boot_alloc_end   = (uintptr_t)&_end; // NOLINT(cppcoreguidelines-interfaces-global-init)

void* boot_alloc_mem(size_t len) {
  uintptr_t ptr  = 0;

  ptr            = ALIGN(boot_alloc_end, 8);
  boot_alloc_end = (ptr + ALIGN(len, 8));

  return (void*)ptr;
}

struct mmu_initial_mapping {
  paddr_t phys;
  vaddr_t virt;
  size_t size; // size == 0 indicate that array of mmu_initial_mapping ends
  unsigned int flags;
  const char* name;
};

constexpr auto SDRAM_BASE           = 0;
/* Note: BCM2836/BCM2837 use different peripheral base than BCM2835 */
constexpr auto BCM_PERIPH_BASE_PHYS = 0x3f000000U;
constexpr auto BCM_PERIPH_SIZE      = 0x01100000U;
constexpr auto BCM_PERIPH_BASE_VIRT = 0xffffffffc0000000ULL;
constexpr auto MEMORY_APERTURE_SIZE = 1024 * 1024 * 1024;
static_assert(MEMORY_APERTURE_SIZE == 1_GiB); // rpi3b has only max 1 GB memory

/* initial memory mappings. parsed by start.S */
extern_C struct mmu_initial_mapping mmu_initial_mappings[] {
  /* 1GB of sdram space */
  {.phys = SDRAM_BASE, .virt = KERNEL_BASE, .size = MEMORY_APERTURE_SIZE, .flags = 0, .name = "memory"},

      /* peripherals */
      {.phys  = BCM_PERIPH_BASE_PHYS,
       .virt  = BCM_PERIPH_BASE_VIRT,
       .size  = BCM_PERIPH_SIZE,
       .flags = MMU_INITIAL_MAPPING_FLAG_DEVICE,
       .name  = "bcm peripherals"},

  /* null entry to terminate the list */
  {
    0, 0, 0, 0, ""
  }
};
