#include "aarch64/mmu.hh"          // for pte_t, MMU_KERNEL_PAGE_SIZE_SHIFT
#include "aarch64/vm/bootalloc.hh" // for boot_alloc_page_phys
#include "arch/defines.hh"         // for ARCH_PHYSIC_MAP_SIZE, PAGE_SIZE
#include "arch/kernel_aspace.hh"   // for KERNEL_ASPACE_BASE
#include "kernel/compiler.hh"      // for __NO_SAFESTACK, __FALLTHROUGH
#include "kernel/errors.hh"        // for ZX_ERR_BAD_STATE, ZX_OK
#include "libcxx/macro.hh"         // for extern_C
#include "libcxx/types.hh"         // for paddr_t, size_t, uintptr_t, vaddr_t
#include "libcxx/unit.hh"          // for _KB

// Static relocated base to prepare for KASLR. Used at early boot and by gdb
// script to know the target relocated address.
// TODO(fxbug.dev/24762): Choose it randomly.
#if CONFIG_KERNEL_ENABLE_KASLR
#error "KASLR currently not implementated"
#else
uint64_t kernel_relocated_base = 0xffff'ffff'0000'0000;
#endif

// The main translation table for the kernel. Globally declared because it's reached from assembly.
// pte_t arm64_kernel_translation_table[512]
constexpr int page_table_size = MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP * 8;
static_assert(page_table_size == 4_K);
pte_t arm64_kernel_translation_table[MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP] __ALIGNED(page_table_size);

// Early boot time page table creation code, called from start.S while running in physical address
// space with the mmu disabled. This code should be position independent as long as it sticks to
// basic code.

// this code only works on a 4K page granule, 48 bits of kernel address space

// 1GB pages
constexpr uintptr_t l1_large_page_size = 1UL << MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 1);
static_assert(l1_large_page_size == 1_GB);
constexpr uintptr_t l1_large_page_size_mask = l1_large_page_size - 1;

// 2MB pages
constexpr uintptr_t l2_large_page_size = 1UL << MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 2);
static_assert(l2_large_page_size == 2_MB);
constexpr uintptr_t l2_large_page_size_mask = l2_large_page_size - 2;

static size_t vaddr_to_l0_index(uintptr_t addr) {
  static_assert(MMU_KERNEL_TOP_SHIFT == 39);
  static_assert(MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP == 512);
  static_assert(MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP - 1 == 0x1FF);
  return (addr >> MMU_KERNEL_TOP_SHIFT) & (MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP - 1);
}

static size_t vaddr_to_l1_index(uintptr_t addr) {
  static_assert(MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 1) == 30);
  static_assert(MMU_KERNEL_PAGE_TABLE_ENTRIES - 1 == 0x1FF);
  return (addr >> MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 1)) & (MMU_KERNEL_PAGE_TABLE_ENTRIES - 1);
}

static size_t vaddr_to_l2_index(uintptr_t addr) {
  static_assert(MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 2) == 21);
  return (addr >> MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 2)) & (MMU_KERNEL_PAGE_TABLE_ENTRIES - 1);
}

static size_t vaddr_to_l3_index(uintptr_t addr) {
  static_assert(MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 3) == 12);
  return (addr >> MMU_LX_X(MMU_KERNEL_PAGE_SIZE_SHIFT, 3)) & (MMU_KERNEL_PAGE_TABLE_ENTRIES - 1);
}

/*// called from start.S to grab another page to back a page table from the boot allocator
__NO_SAFESTACK
extern_C pte_t* boot_alloc_ptable() {
  // allocate a page out of the boot allocator, asking for a physical address
  pte_t* ptr = reinterpret_cast<pte_t*>(boot_alloc_page_phys());

  // avoid using memset, since this relies on dc zva instruction, which isn't set up at this point in the boot process
  // use a volatile pointer to make sure
  volatile pte_t* vptr = ptr;
  for (auto i = 0; i < MMU_KERNEL_PAGE_TABLE_ENTRIES; i++) {
    vptr[i] = 0;
  }

  return ptr;
}*/

// inner mapping routine passed two helper routines
__NO_SAFESTACK
inline zx_status_t _arm64_boot_map(pte_t* page_table, const vaddr_t vaddr, const paddr_t paddr, const size_t len,
                                   const pte_t flags, paddr_t (*alloc_func)(), pte_t* phys_to_virt(paddr_t)) {
  // loop through the virtual range and map each physical page, using the largest
  // page size supported. Allocates necessary page tables along the way.
  size_t offset = 0;
  while (offset < len) {
    // make sure the level 1 pointer is valid
    size_t index0        = vaddr_to_l0_index(vaddr + offset);
    pte_t* kernel_table1 = nullptr;
    switch (page_table[index0] & MMU_PTE_DESCRIPTOR_MASK) {
    default: { // invalid/unused entry
      paddr_t pa = alloc_func();

      page_table[index0] = (pa & MMU_PTE_OUTPUT_ADDR_MASK) | MMU_PTE_L012_DESCRIPTOR_TABLE;
      __FALLTHROUGH;
    }
    case MMU_PTE_L012_DESCRIPTOR_TABLE:
      kernel_table1 = phys_to_virt(page_table[index0] & MMU_PTE_OUTPUT_ADDR_MASK);
      break;
    case MMU_PTE_L012_DESCRIPTOR_BLOCK:
      // not legal to have a block pointer at this level
      return ZX_ERR_BAD_STATE;
    }

    // make sure the level 2 pointer is valid
    size_t index1        = vaddr_to_l1_index(vaddr + offset);
    pte_t* kernel_table2 = nullptr;
    switch (kernel_table1[index1] & MMU_PTE_DESCRIPTOR_MASK) {
    default: { // invalid/unused entry
      // a large page at this level is 1GB long, see if we can make one here
      if ((((vaddr + offset) & l1_large_page_size_mask) == 0) && (((paddr + offset) & l1_large_page_size_mask) == 0) &&
          (len - offset) >= l1_large_page_size) {
        // set up a 1GB page here
        kernel_table1[index1] = ((paddr + offset) & ~l1_large_page_size_mask) | flags | MMU_PTE_L012_DESCRIPTOR_BLOCK;

        offset += l1_large_page_size;
        continue;
      }

      paddr_t pa = alloc_func();

      kernel_table1[index1] = (pa & MMU_PTE_OUTPUT_ADDR_MASK) | MMU_PTE_L012_DESCRIPTOR_TABLE;
      __FALLTHROUGH;
    }
    case MMU_PTE_L012_DESCRIPTOR_TABLE:
      kernel_table2 = phys_to_virt(kernel_table1[index1] & MMU_PTE_OUTPUT_ADDR_MASK);
      break;
    case MMU_PTE_L012_DESCRIPTOR_BLOCK:
      // not legal to have a block pointer at this level
      return ZX_ERR_BAD_STATE;
    }

    // make sure the level 3 pointer is valid
    size_t index2        = vaddr_to_l2_index(vaddr + offset);
    pte_t* kernel_table3 = nullptr;
    switch (kernel_table2[index2] & MMU_PTE_DESCRIPTOR_MASK) {
    default: { // invalid/unused entry
      // a large page at this level is 2MB long, see if we can make one here
      if ((((vaddr + offset) & l2_large_page_size_mask) == 0) && (((paddr + offset) & l2_large_page_size_mask) == 0) &&
          (len - offset) >= l2_large_page_size) {
        // set up a 2MB page here
        kernel_table2[index2] = ((paddr + offset) & ~l2_large_page_size_mask) | flags | MMU_PTE_L012_DESCRIPTOR_BLOCK;

        offset += l2_large_page_size;
        continue;
      }

      paddr_t pa = alloc_func();

      kernel_table2[index2] = (pa & MMU_PTE_OUTPUT_ADDR_MASK) | MMU_PTE_L012_DESCRIPTOR_TABLE;
      __FALLTHROUGH;
    }
    case MMU_PTE_L012_DESCRIPTOR_TABLE:
      kernel_table3 = phys_to_virt(kernel_table2[index2] & MMU_PTE_OUTPUT_ADDR_MASK);
      break;
    case MMU_PTE_L012_DESCRIPTOR_BLOCK:
      // not legal to have a block pointer at this level
      return ZX_ERR_BAD_STATE;
    }

    // generate a standard page mapping
    size_t index3         = vaddr_to_l3_index(vaddr + offset);
    kernel_table3[index3] = ((paddr + offset) & MMU_PTE_OUTPUT_ADDR_MASK) | flags | MMU_PTE_L3_DESCRIPTOR_PAGE;

    offset += PAGE_SIZE;
  }

  return ZX_OK;
}

// called from start.S to configure level 1-3 page tables to map the kernel wherever it is located
// physically to KERNEL_BASE
__NO_SAFESTACK
extern_C zx_status_t arm64_boot_map(pte_t* kernel_table0, const vaddr_t vaddr, const paddr_t paddr, const size_t len,
                                    const pte_t flags) {
  // the following helper routines assume that code is running in physical addressing mode (mmu
  // off). any physical addresses calculated are assumed to be the same as virtual
  auto alloc = []() __NO_SAFESTACK -> paddr_t {
    // allocate a page out of the boot allocator, asking for a physical address
    paddr_t pa = boot_alloc_page_phys();

    // avoid using memset, since this relies on dc zva instruction, which isn't set up at
    // this point in the boot process
    // use a volatile pointer to make sure the compiler doesn't emit a memset call
    volatile pte_t* vptr = reinterpret_cast<volatile pte_t*>(pa);
    for (auto i = 0; i < MMU_KERNEL_PAGE_TABLE_ENTRIES; i++) {
      vptr[i] = 0;
    }

    return pa;
  };

  auto phys_to_virt = [](paddr_t pa) __NO_SAFESTACK -> pte_t* { return reinterpret_cast<pte_t*>(pa); };

  return _arm64_boot_map(kernel_table0, vaddr, paddr, len, flags, alloc, phys_to_virt);
}

// todo: setup boot mmu here
extern_C void arm64_mmu_boot_mapping_setup() {
  //  mov     x0, page_table1
  //  mov     x1, KERNEL_ASPACE_BASE
  //  mov     x2, 0
  //  mov     x3, ARCH_PHYSMAP_SIZE
  //  movlit  x4, MMU_PTE_KERNEL_DATA_FLAGS
  arm64_boot_map(arm64_kernel_translation_table, KERNEL_ASPACE_BASE, 0, ARCH_PHYSIC_MAP_SIZE,
                 MMU_PTE_KERNEL_DATA_FLAGS);
  //  mov     x0, page_table1
  //  mov     x1, kernel_vaddr    // in fuchsia: 0xffffffff00000000
  //  adr_global x2, __kernel_start // in fuchsia: 0xffffffff00000000
  //  adr_global x3, __kernel_end
  //  sub     x3, x3, x2
  //  mov     x4, MMU_PTE_KERNEL_RWX_FLAGS
  //  bl      arm64_boot_map
  extern paddr_t __kernel_start[];
  extern paddr_t __kernel_end[];
  arm64_boot_map(arm64_kernel_translation_table, static_cast<vaddr_t>(kernel_relocated_base),
                 reinterpret_cast<paddr_t>(__kernel_start), __kernel_end - __kernel_start, MMU_PTE_KERNEL_RWX_FLAGS);
}

#if 0
// called a bit later in the boot process once the kernel is in virtual memory to map early kernel
// data
zx_status_t arm64_boot_map_v(const vaddr_t vaddr, const paddr_t paddr, const size_t len,
                             const pte_t flags) {
  // assumed to be running with virtual memory enabled, so use a slightly different set of routines
  // to allocate and find the virtual mapping of memory
  auto alloc = []() -> paddr_t {
    // allocate a page out of the boot allocator, asking for a physical address
    paddr_t pa = boot_alloc_page_phys();

    // zero the memory using the physmap
    void* ptr = paddr_to_physmap(pa);
    memset(ptr, 0, MMU_KERNEL_PAGE_TABLE_ENTRIES * sizeof(pte_t));

    return pa;
  };

  auto phys_to_virt = [](paddr_t pa) -> pte_t* {
    return reinterpret_cast<pte_t*>(paddr_to_physmap(pa));
  };

  return _arm64_boot_map(arm64_get_kernel_ptable(), vaddr, paddr, len, flags, alloc, phys_to_virt);
}
#endif

// todo: refer to lk source code, and use struct initial_mapping to setup mmu address mapping
