#include "arm64/asm.hpp"
#include "arm64/barrier.hpp"
#include "arm64/mair.hpp"
#include "arm64/pgtable.hpp"
#include "arm64/pgtable_hwdef.hpp"
#include "arm64/pgtable_prot.hpp"
#include "arm64/sysregs.hpp"
#include "libc/log.hpp"

#define BIT(nr)           (1UL << (nr))
#define NO_BLOCK_MAPPINGS BIT(0)
#define NO_CONT_MAPPINGS  BIT(1)

extern l0_page_table l0_page_table_base_address;
static const l0_page_table *l0 = &l0_page_table_base_address;

static void l3_mapping(pmd_t *pmdp, unsigned long addr, unsigned long end, unsigned long phys, unsigned long prot,
                           unsigned long (*alloc_pgtable)(void), unsigned long flags) {
  pte_t *ptep = nullptr;

  if (pmdp->pmd == 0U) {
    u64 pte_phys = alloc_pgtable();
    //    set_pmd(pmdp, __pmd(pte_phys | PMD_TYPE_TABLE));
    pmdp->pmd = pte_phys | PMD_TYPE_TABLE;
    dsb(ishst);
  }

  /// [20:12]
  ptep = ((pte_t *)((pmdp->pmd & 0xffff'ffff'f000) + ((addr >> 12) & 0x1ff) * sizeof(pte_t)));
  //  ptep = pte_offset_phys(pmdp, addr);
  log_i("pte index: 0x%llx", pte_index(addr));
  do {
    //    set_pte(ptep, pfn_pte(phys >> PAGE_SHIFT, prot));
//    ptep->pte = (((phys >> 12) << 12) | (prot));
    ptep->pte = (phys & 0xFFFF'FFFF'FFFF'F000) | prot;
    dsb(ishst);

    phys += PAGE_SIZE;
    ptep++;
    addr += PAGE_SIZE;
  } while (addr != end);
}

void pmd_set_section(pmd_t *pmdp, unsigned long phys, unsigned long prot) {
  unsigned long sect_prot = PMD_TYPE_SECT | mk_sect_prot(prot);

  pmd_t new_pmd = pfn_pmd(phys >> PMD_SHIFT, sect_prot);

  set_pmd(pmdp, new_pmd);
}
static u64 l2_address_end(u64 addr, u64 end) {
  u64 boundary = (addr + (1UL << 21)) & (~((1UL << 21) - 1));
  return boundary - 1 < end - 1 ? boundary : end;
}
static void l2_mapping(pud_t *pudp, unsigned long addr, unsigned long end, unsigned long phys, unsigned long prot,
                           unsigned long (*alloc_pgtable)(void), unsigned long flags) {
  if (pudp->pud == 0U) {
    u64 pmd_phys = alloc_pgtable();
    //    set_pud(pudp, __pud(pmd_phys | PUD_TYPE_TABLE));
    pudp->pud = pmd_phys | PUD_TYPE_TABLE;
    dsb(ishst);
  }

  /// [29:21]
  pmd_t *pmdp = ((pmd_t *)((pudp->pud & 0xffff'ffff'f000) + (((addr >> 21) & 0x1ff) * sizeof(pmd_t))));
  //  pmd_t *pmdp = pmd_offset_phys(pudp, addr);
  log_i("pmdp: %p, pmd index 0x%llx", pmdp, pmd_index(addr));
  u64 next = 0;
  do {
//    next = pmd_addr_end(addr, end);
    next = l2_address_end(addr, end);

    if (((addr | next | phys) & ~SECTION_MASK) == 0 && (flags & NO_BLOCK_MAPPINGS) == 0) {
      pmd_set_section(pmdp, phys, prot);
    } else {
      l3_mapping(pmdp, addr, next, phys, prot, alloc_pgtable, flags);
    }

    phys += next - addr;
    pmdp++;
    addr = next;
  } while (addr != end);
}
static u64 l1_address_end(u64 virtual_address, u64 end) {
  u64 boundary = ((virtual_address) + (1UL << 30)) & (~((1UL << 30) - 1));
  return boundary - 1 < end - 1 ? boundary : end;
}
static void l1_mapping(l0_page_table_entry_t *pgdp, unsigned long virtual_address, unsigned long end,
                           unsigned long phys, unsigned long prot, unsigned long (*alloc_pgtable)(void),
                           unsigned long flags) {
  if (pgdp->pgd == 0U) {
    u64 pud_phys = alloc_pgtable();
    log_i("pud_phys is: 0x%llx", pud_phys);

    pgdp->pgd = pud_phys | PUD_TYPE_TABLE;
    dsb(ishst);
    log_i("pgdp is: 0x%llx", pgdp->pgd);
  }

  /// [38:30]
  pud_t *pudp = (pud_t *)(pgdp->pgd & 0xffff'ffff'f000) + ((virtual_address >> 30) & 0x1ff) * sizeof(pud_t);
  //  pud_t *pudp = pud_offset_phys(pgdp, addr);
  log_i("pudp index: 0x%llx", pud_index(virtual_address));
  u64 next = 0;
  do {
    //    next = pud_addr_end(virtual_address, end);
    next = l1_address_end(virtual_address, end);
    l2_mapping(pudp, virtual_address, next, phys, prot, alloc_pgtable, flags);
    phys += next - virtual_address;
    pudp++;
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

static inline l0_page_table_entry_t *locate_l0_entry_offset(l0_page_table_entry_t *l0_page_table_address,
                                                            u64 virtual_addr) {
  return (l0_page_table_address + l0_page_table_index(virtual_addr));
}
static u64 l0_address_end(u64 virtual_address, u64 end) {
  u64 boundary = (virtual_address + (1UL << 39)) & (~((1UL << 39) - 1));
  return boundary - 1 < end - 1 ? boundary : end;
};

static void create_l0_mapping(l0_page_table_entry_t *l0_page_table_address, unsigned long phys, unsigned long virt,
                              unsigned long size, unsigned long prot, unsigned long (*alloc_pgtable)(void),
                              unsigned long flags) {
  log_i("prot is: 0x%llx", prot);
  l0_page_table_entry_t *l0_page_table_entry = locate_l0_entry_offset(l0_page_table_address, virt);

  log_i("phys is: 0x%llx", phys);
  //  phys &= PAGE_MASK;
  phys &= 0xffff'ffff'ffff'f000;
  log_i("phys is: 0x%llx", phys);
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
    l1_mapping(l0_page_table_entry, virtual_address, next, phys, prot, alloc_pgtable, flags);
    phys += next - virtual_address;
    l0_page_table_entry++;
    virtual_address = next;
  } while (virtual_address != end);
}

extern unsigned long get_free_page();
static unsigned long early_pgtable_alloc() {
  u64 phys = get_free_page();
  memset((void *)phys, 0, PAGE_SIZE);

  return phys;
}

extern u64 __text_start;
extern u64 __text_end;

static void create_identical_mapping() {
  /*map text*/
  u64 start = (u64)&__text_start;
  log_i("text start is: 0x%p", &__text_start);
  log_i("start is: 0x%p", start);
  u64 end = (u64)&__text_end;
  log_i("text end is: 0x%p", &__text_end);
  log_i("end is: 0x%p", end);
  log_i("PAGE_KERNEL_ROX is: 0x%llx", PAGE_KERNEL_ROX);
  log_i("page_kernel_rox is: 0x%llx", PAGE_KERNEL_ROX);
  log_i("pk_rox is: 0x%llx", PAGE_KERNEL_ROX);
  create_l0_mapping((l0_page_table_entry_t *)&l0_page_table_base_address, start, start, end - start, PAGE_KERNEL_ROX,
                    early_pgtable_alloc, 0);
  log_i("text start => end: 0x%llx => 0x%llx", start, end);
  // todo: map rodata section, data section

  /*map memory*/
  start = PAGE_ALIGN((unsigned long)&__text_end);
  end   = TOTAL_MEMORY;
  log_i("memory start => end: 0x%llx => 0x%llx", start, end);
  log_i("PAGE_KERNEL is: 0x%llx", PAGE_KERNEL);
  create_l0_mapping((l0_page_table_entry_t *)&l0_page_table_base_address, start, start, end - start, PAGE_KERNEL,
                    early_pgtable_alloc, 0);
}

static void create_mmio_mapping() {
  //  __create_pgd_mapping((pgd_t *)idmap_pg_dir, PBASE, PBASE,
  //                       DEVICE_SIZE, PROT_DEVICE_nGnRnE,
  //                       early_pgtable_alloc,
  //                       0);
  // NOTE: DEVICE_SIZE must be big enough to map all PERIPHERAL_BASE and LOCAL_PERIPHERALS_BASE
  create_l0_mapping((l0_page_table_entry_t *)&l0_page_table_base_address, 0x3F00'0000, 0x3F00'0000,
                    /*DEVICE_SIZE*/ 0x10000000, PROT_DEVICE_nGnRnE, early_pgtable_alloc, 0);
}

static void cpu_init() {
  asm("tlbi vmalle1");
  dsb(nsh);

  write_sysreg(3UL << 20, cpacr_el1);
  write_sysreg(1 << 12, mdscr_el1);

  setup_mair_el1();

  u64 tcr = TCR_TxSZ(VA_BITS) | TCR_TG_FLAGS;

  u64 tmp    = read_sysreg(ID_AA64MMFR0_EL1);
  u64 parang = tmp & 0xf;
  if (parang > ID_AA64MMFR0_PARANGE_48) {
    parang = ID_AA64MMFR0_PARANGE_48;
  }

  tcr |= parang << TCR_IPS_SHIFT;

  write_sysreg(tcr, tcr_el1);
}

static int enable_mmu() {
  u64 tmp    = read_sysreg(ID_AA64MMFR0_EL1);
  u64 tgran4 = (tmp >> ID_AA64MMFR0_TGRAN4_SHIFT) & 0xf;
  log_i("tgran4 is: %llu", tgran4);
  if (tgran4 != ID_AA64MMFR0_TGRAN4_SUPPORTED) {
    return -1;
  }

  write_sysreg(&l0_page_table_base_address, ttbr0_el1);
  isb();

  write_sysreg(SCTLR_ELx_M, sctlr_el1);
  isb();
  asm("ic iallu");
  dsb(nsh);
  isb();

  return 0;
}

void paging_init() {
  log_i("l0_page_table is: 0x%p", &l0_page_table_base_address);
  // NOTE: PAGE_SIZE must be same with the value defined in the linker script
  memset(&l0_page_table_base_address, 0, PAGE_SIZE);
  create_identical_mapping();
  create_mmio_mapping();
  cpu_init();
  enable_mmu();

  log_i("enable mmu done");
}

void init_mmu() { paging_init(); }
