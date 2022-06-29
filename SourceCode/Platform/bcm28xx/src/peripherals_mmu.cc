#include "aarch64/boot_mmu.hh" // for arm64_boot_map
#include "aarch64/mmu.hh"      // for MMU_INITIAL_MAP_DEVICE, arm64_kernel_...
#include "bcm28xx/bcm28xx.hh"  // for BCM_PERIPH_BASE_PHYS, BCM_PERIPH_BASE...
#include "libcxx/attr.hh"      // for attr_used
#include "libcxx/macros.hh"    // for extern_C
#include "libcxx/types.hh"     // for i64, pte_t

extern_C attr_used i64 arm64_boot_map_platform_peripherals(pte_t* kernel_table0) {
  // NOTE: don't pass static_cast<pte_t*>(arm64_kernel_translation_table) as first parameter,
  // the compiler generated code is not correct because the load address defined in linker script is 0xffffffff00000000,
  // but the actual address of arm64_kernel_translation_table is starting 0x80000(rpi3)
  return arm64_boot_map(kernel_table0, BCM_PERIPH_BASE_VIRT, BCM_PERIPH_BASE_PHYS, MEMORY_APERTURE_SIZE,
                        MMU_INITIAL_MAP_DEVICE);
}
