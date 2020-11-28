#include "arm64/asm.hpp"
#include "arm64/barrier.hpp"
#include "arm64/mair.hpp"
#include "arm64/page.hpp"
#include "arm64/pgtable.hpp"
#include "arm64/pgtable_hwdef.hpp"
#include "arm64/pgtable_prot.hpp"
#include "arm64/sysregs.hpp"
#include "libcxx/log.hpp"

#define BIT(nr)           (1UL << (nr))
#define NO_BLOCK_MAPPINGS BIT(0)
#define NO_CONT_MAPPINGS  BIT(1)

/// the l0 page table
/*[[gnu::aligned(PAGE_SIZE)]]*/ static l0_page_table l0_page_table_base_address __attribute__((aligned(PAGE_SIZE)));

/// pointer to function to allocate page table
using func_allocate_page = u64 (*)();

u64 locate_l3_entry_offset(u64 virtual_address) { return (((virtual_address) >> PAGE_SHIFT) & (PTRS_PER_PTE - 1)); }
static void l3_mapping(l2_page_table_entry *l2_page_table_entry, u64 virtual_address, u64 end, u64 physical_address,
                       u64 property, func_allocate_page alloc_page, u64 flags) {
  //  l3_page_table_entry *l3_page_table_entry = nullptr;

  if (*l2_page_table_entry == 0U) {
    u64 l3_physic_address = alloc_page();
    //    set_pmd(pmdp, __pmd(pte_phys | PMD_TYPE_TABLE));
    *l2_page_table_entry = l3_physic_address | PMD_TYPE_TABLE;
    dsb(ishst);
  }

  /// [20:12]
  l3_page_table_entry *l3_page_table_entry_ =
      (l3_page_table_entry *)((*l2_page_table_entry & 0xffff'ffff'f000) +
                              ((virtual_address >> 12) & 0x1ff) * sizeof(l3_page_table_entry));
  //  ptep = pte_offset_phys(pmdp, addr);
  log_i("pte index: 0x%llx", locate_l3_entry_offset(virtual_address));
  do {
    //    set_pte(ptep, pfn_pte(phys >> PAGE_SHIFT, prot));
    //    ptep->pte = (((phys >> 12) << 12) | (prot));
    *l3_page_table_entry_ = (physical_address & 0xFFFF'FFFF'FFFF'F000) | property;
    dsb(ishst);

    physical_address += PAGE_SIZE;
    l3_page_table_entry_++;
    virtual_address += PAGE_SIZE;
  } while (virtual_address != end);
}

void l2_set_section(l2_page_table_entry *pmdp, u64 phys, u64 prot) {
  u64 sect_prot = PMD_TYPE_SECT | mk_sect_prot(prot);

  //  pmd_t new_pmd = pfn_pmd(phys >> PMD_SHIFT, sect_prot);
  l2_page_table_entry new_pmd = ((phys >> PMD_SHIFT) << PMD_SHIFT) | (sect_prot);

  //  set_pmd(pmdp, new_pmd);
  *pmdp = new_pmd;

  dsb(ishst);
}
static u64 l2_address_end(u64 addr, u64 end) {
  u64 boundary = (addr + (1UL << 21)) & (~((1UL << 21) - 1));
  return boundary - 1 < end - 1 ? boundary : end;
}

u64 locate_l2_entry_offset(u64 addr) { return ((addr) >> PMD_SHIFT & (PTRS_PER_PMD - 1)); }
static void l2_mapping(l1_page_table_entry *l1_page_table_entry, u64 virtual_address, u64 end, u64 physical_address,
                       u64 property, func_allocate_page alloc_page, u64 flags) {
  if (*l1_page_table_entry == 0U) {
    u64 l2_physic_address = alloc_page();
    //    set_pud(pudp, __pud(pmd_phys | PUD_TYPE_TABLE));
    *l1_page_table_entry = l2_physic_address | PUD_TYPE_TABLE;
    dsb(ishst);
  }

  /// [29:21]
  l2_page_table_entry *l2_page_table_entry_ =
      ((l2_page_table_entry *)((*l1_page_table_entry & 0xffff'ffff'f000) +
                               (((virtual_address >> 21) & 0x1ff) * sizeof(l2_page_table_entry_))));
  //  pmd_t *pmdp = pmd_offset_phys(pudp, addr);
  log_i("pmdp: %p, pmd index 0x%llx", l2_page_table_entry_, locate_l2_entry_offset(virtual_address));
  u64 next = 0;
  do {
    //    next = pmd_addr_end(addr, end);
    next = l2_address_end(virtual_address, end);

    if (((virtual_address | next | physical_address) & ~SECTION_MASK) == 0 && (flags & NO_BLOCK_MAPPINGS) == 0) {
      l2_set_section(l2_page_table_entry_, physical_address, property);
    } else {
      l3_mapping(l2_page_table_entry_, virtual_address, next, physical_address, property, alloc_page, flags);
    }

    physical_address += next - virtual_address;
    l2_page_table_entry_++;
    virtual_address = next;
  } while (virtual_address != end);
}
static u64 l1_address_end(u64 virtual_address, u64 end) {
  u64 boundary = ((virtual_address) + (1UL << 30)) & (~((1UL << 30) - 1));
  return boundary - 1 < end - 1 ? boundary : end;
}

u64 locate_l1_entry_offset(u64 virtual_address) { return ((virtual_address) >> PUD_SHIFT & (PTRS_PER_PUD - 1)); }
static void l1_mapping(l0_page_table_entry *l0_page_table_entry, u64 virtual_address, u64 end, u64 physical_address,
                       u64 property, func_allocate_page alloc_page, u64 flags) {
  if (*l0_page_table_entry == 0U) {
    u64 l1_physic_address = alloc_page();
    log_i("pud_phys is: 0x%llx", l1_physic_address);

    *l0_page_table_entry = l1_physic_address | PUD_TYPE_TABLE;
    dsb(ishst);
    log_i("pgdp is: 0x%llx", *l0_page_table_entry);
  }

  /// [38:30]
  l1_page_table_entry *l1_page_table_entry_ = (l1_page_table_entry *)(*l0_page_table_entry & 0xffff'ffff'f000) +
                                              ((virtual_address >> 30) & 0x1ff) * sizeof(l1_page_table_entry);
  //  pud_t *pudp = pud_offset_phys(pgdp, addr);
  log_i("pudp index: 0x%llx", locate_l1_entry_offset(virtual_address));
  u64 next = 0;
  do {
    //    next = pud_addr_end(virtual_address, end);
    next = l1_address_end(virtual_address, end);
    l2_mapping(l1_page_table_entry_, virtual_address, next, physical_address, property, alloc_page, flags);
    physical_address += next - virtual_address;
    l1_page_table_entry_++;
    virtual_address = next;
  } while (virtual_address != end);
}

static inline u64 l0_page_table_index(u64 virtual_address) {
  log_d("address is 0x%llx", virtual_address);
  //  u64 offset = (address >> 39) & (1 << 8);
  /// virtual address offset in l0
  //  u64 offset = (virtual_address >> 39) & (0b1'1111'1111);
  /// [47:39] bits
  // TODO: confirm 0x0000'FF80'0000'0000
  u64 offset = virtual_address & 0x0000'FF80'0000'0000;
  log_d("l0 page table offset is: 0x%llx", offset);
  return offset;
}

static inline l0_page_table_entry *locate_l0_entry_offset(l0_page_table_entry *l0_page_table_address,
                                                          u64 virtual_addr) {
  return (l0_page_table_address + l0_page_table_index(virtual_addr));
}
static u64 l0_address_end(u64 virtual_address, u64 end) {
  u64 boundary = (virtual_address + (1UL << 39)) & (~((1UL << 39) - 1));
  return boundary - 1 < end - 1 ? boundary : end;
};

static void create_l0_mapping(l0_page_table_entry *l0_page_table_address, u64 physical_address, u64 virt, u64 size,
                              u64 property, func_allocate_page alloc_page, u64 flags) {
  log_i("prot is: 0x%llx", property);
  l0_page_table_entry *l0_page_table_entry = locate_l0_entry_offset(l0_page_table_address, virt);

  log_i("phys is: 0x%llx", physical_address);
  //  phys &= PAGE_MASK;
  physical_address &= 0xffff'ffff'ffff'f000;
  log_i("phys is: 0x%llx", physical_address);
  log_i("virt is: 0x%llx", virt);
  //  u64 virtual_address = virt & PAGE_MASK;
  u64 virtual_address = virt & 0xffff'ffff'ffff'f000;
  log_i("addr is: 0x%llx", virtual_address);
  log_i("virt + size is: 0x%llx", virt + size);
  u64 end = PAGE_ALIGN(virt + size);
  log_i("end is: 0x%llx", end);

  u64 next = 0;
  do {
    log_i("l0 do while");
    //    next = pgd_addr_end(virtual_address, end);
    next = l0_address_end(virtual_address, end);
    l1_mapping(l0_page_table_entry, virtual_address, next, physical_address, property, alloc_page, flags);
    physical_address += next - virtual_address;
    l0_page_table_entry++;
    virtual_address = next;
  } while (virtual_address != end);
}

extern u64 get_free_page();
static u64 early_allocate_page_table() {
  u64 phys = get_free_page();
  memset((void *)phys, 0, PAGE_SIZE);

  return phys;
}
func_allocate_page default_func_allocate_page = &early_allocate_page_table;

extern u64 __text_start;
extern u64 __text_end;

static void create_identical_mapping() {
  /*map text*/
  u64 start = (u64)(&__text_start);
  u64 end   = (u64)&__text_end;
  log_i("PAGE_KERNEL_ROX is: 0x%llx", PAGE_KERNEL_ROX);
  log_i("page_kernel_rox is: 0x%llx", PAGE_KERNEL_ROX);
  log_i("pk_rox is: 0x%llx", PAGE_KERNEL_ROX);
  create_l0_mapping((l0_page_table_entry *)&l0_page_table_base_address, start, start, end - start, PAGE_KERNEL_ROX,
                    default_func_allocate_page, 0);
  log_i("text start => end: 0x%llx => 0x%llx", start, end);
  // todo: map rodata section, data section

  /*map memory*/
  start = PAGE_ALIGN((u64)&__text_end);
  end   = TOTAL_MEMORY;
  log_i("memory start => end: 0x%llx => 0x%llx", start, end);
  log_i("PAGE_KERNEL is: 0x%llx", PAGE_KERNEL);
  create_l0_mapping((l0_page_table_entry *)&l0_page_table_base_address, start, start, end - start, PAGE_KERNEL,
                    default_func_allocate_page, 0);
}

static void create_mmio_identical_mapping() {
  //  __create_pgd_mapping((pgd_t *)idmap_pg_dir, PBASE, PBASE,
  //                       DEVICE_SIZE, PROT_DEVICE_nGnRnE,
  //                       default_func_allocate_page,
  //                       0);
  // NOTE: DEVICE_SIZE must be big enough to map all PERIPHERAL_BASE and LOCAL_PERIPHERALS_BASE
  create_l0_mapping((l0_page_table_entry *)&l0_page_table_base_address, 0x3F00'0000, 0x3F00'0000,
                    /*DEVICE_SIZE*/ 0x10000000, PROT_DEVICE_nGnRnE, default_func_allocate_page, 0);
}

static void cpu_init() {
  asm volatile("tlbi vmalle1");
  dsb(nsh);

  ARM64_WRITE_SYSTEM_REGISTER(cpacr_el1, 3UL << 20);
  ARM64_WRITE_SYSTEM_REGISTER(mdscr_el1, 1UL << 12);

  setup_mair_el1();

  u64 tcr = TCR_TxSZ(VA_BITS) | TCR_TG_FLAGS;

  u64 tmp    = ARM64_READ_SYSTEM_REGISTER(ID_AA64MMFR0_EL1);
  u64 parang = tmp & 0xf;
  if (parang > ID_AA64MMFR0_PARANGE_48) {
    parang = ID_AA64MMFR0_PARANGE_48;
  }

  tcr |= parang << TCR_IPS_SHIFT;

  ARM64_WRITE_SYSTEM_REGISTER(tcr_el1, tcr);
}

static int enable_mmu() {
  u64 tmp    = ARM64_READ_SYSTEM_REGISTER(ID_AA64MMFR0_EL1);
  u64 tgran4 = (tmp >> ID_AA64MMFR0_TGRAN4_SHIFT) & 0xf;
  log_i("tgran4 is: %llu", tgran4);
  if (tgran4 != ID_AA64MMFR0_TGRAN4_SUPPORTED) {
    log_e("tgran4 is not supported, tgran4 is: %ll", tgran4);
    return -1;
  }

  ARM64_WRITE_SYSTEM_REGISTER(ttbr0_el1, &l0_page_table_base_address);
  isb();

  ARM64_WRITE_SYSTEM_REGISTER(sctlr_el1, SCTLR_ELx_M);
  isb();
  asm volatile("ic iallu");
  dsb(nsh);
  isb();

  return 0;
}

void paging_init() {
  log_i("l0_page_table is: 0x%p", &l0_page_table_base_address);
  // &l0_page_table_address is in bss section, it's already be cleaned before, there's no need to clean it again
  // memset(&&l0_page_table_address, 0, PAGE_SIZE);
  create_identical_mapping();
  create_mmio_identical_mapping();
  cpu_init();
  enable_mmu();

  log_i("enable mmu done");
}

void init_mmu() { paging_init(); }
