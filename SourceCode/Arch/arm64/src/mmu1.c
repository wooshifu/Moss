#include "arm64/asm.h"
#include "arm64/barrier.h"
#include "arm64/mair.h"
#include "arm64/pgtable.h"
#include "arm64/pgtable_hwdef.h"
#include "arm64/pgtable_prot.h"
#include "arm64/sysregs.h"
#include "libc/log.h"

#define BIT(nr)           (1UL << (nr))
#define NO_BLOCK_MAPPINGS BIT(0)
#define NO_CONT_MAPPINGS  BIT(1)

extern u64 l0_page_table;

static void alloc_init_pte(pmd_t *pmdp, unsigned long addr, unsigned long end, unsigned long phys, unsigned long prot,
                           unsigned long (*alloc_pgtable)(void), unsigned long flags) {
  pte_t *ptep = NULL;

  if (pmd_none(*pmdp)) {
    u64 pte_phys = alloc_pgtable();
    set_pmd(pmdp, __pmd(pte_phys | PMD_TYPE_TABLE));
  }

  ptep = pte_offset_phys(pmdp, addr);
  do {
    set_pte(ptep, pfn_pte(phys >> PAGE_SHIFT, prot));
    phys += PAGE_SIZE;
  } while (ptep++, addr += PAGE_SIZE, addr != end);
}

void pmd_set_section(pmd_t *pmdp, unsigned long phys, unsigned long prot) {
  unsigned long sect_prot = PMD_TYPE_SECT | mk_sect_prot(prot);

  pmd_t new_pmd = pfn_pmd(phys >> PMD_SHIFT, sect_prot);

  set_pmd(pmdp, new_pmd);
}

static void alloc_init_pmd(pud_t *pudp, unsigned long addr, unsigned long end, unsigned long phys, unsigned long prot,
                           unsigned long (*alloc_pgtable)(void), unsigned long flags) {
  if (pud_none(*pudp)) {
    u64 pmd_phys = alloc_pgtable();
    set_pud(pudp, __pud(pmd_phys | PUD_TYPE_TABLE));
  }

  pmd_t *pmdp = pmd_offset_phys(pudp, addr);
  u64 next    = 0;
  do {
    next = pmd_addr_end(addr, end);

    if (((addr | next | phys) & ~SECTION_MASK) == 0 && (flags & NO_BLOCK_MAPPINGS) == 0) {
      pmd_set_section(pmdp, phys, prot);
    } else {
      alloc_init_pte(pmdp, addr, next, phys, prot, alloc_pgtable, flags);
    }

    phys += next - addr;
  } while (pmdp++, addr = next, addr != end);
}

static void alloc_init_pud(l0_page_table_entry_t *pgdp, unsigned long addr, unsigned long end, unsigned long phys,
                           unsigned long prot, unsigned long (*alloc_pgtable)(void), unsigned long flags) {
  if (pgd_none(*pgdp)) {
    u64 pud_phys = alloc_pgtable();

    set_pgd(pgdp, __pgd(pud_phys | PUD_TYPE_TABLE));
  }

  pud_t *pudp = pud_offset_phys(pgdp, addr);
  u64 next    = 0;
  do {
    next = pud_addr_end(addr, end);
    alloc_init_pmd(pudp, addr, next, phys, prot, alloc_pgtable, flags);
    phys += next - addr;

  } while (pudp++, addr = next, addr != end);
}

static inline u64 l0_index(u64 address) {
  log_d("address is 0x%llx", address);
  /// [47:39]
  u64 offset = (address >> 39) & (1 << 8);
  log_d("offset is: 0x%llx", offset);
  return offset;
}

static inline l0_page_table_entry_t *locate_l0_entry_offset(l0_page_table_entry_t *l0_page_table_address,
                                                            u64 virtual_addr) {
  return (l0_page_table_address + l0_index(virtual_addr));
}

static void create_l0_mapping(l0_page_table_entry_t *l0_page_table_address, unsigned long phys, unsigned long virt,
                              unsigned long size, unsigned long prot, unsigned long (*alloc_pgtable)(void),
                              unsigned long flags) {
  log_i("prot is: 0x%llx", prot);
  l0_page_table_entry_t *l0_page_table_entry = locate_l0_entry_offset(l0_page_table_address, virt);

  phys &= PAGE_MASK;
  u64 addr = virt & PAGE_MASK;
  u64 end  = PAGE_ALIGN(virt + size);

  u64 next = 0;
  do {
    next = pgd_addr_end(addr, end);
    alloc_init_pud(l0_page_table_entry, addr, next, phys, prot, alloc_pgtable, flags);
    phys += next - addr;
  } while (l0_page_table_entry++, addr = next, addr != end);
}

extern unsigned long get_free_page(void);
static unsigned long early_pgtable_alloc(void) {
  u64 phys = get_free_page();
  memset((void *)phys, 0, PAGE_SIZE);

  return phys;
}

extern u64 __text_start;
extern u64 __text_end;

static void create_identical_mapping(void) {
  /*map text*/
  u64 start = (u64)&__text_start;
  log_i("text start is: 0x%p", &__text_start);
  log_i("start is: 0x%p", start);
  u64 end = (u64)&__text_end;
  log_i("text end is: 0x%p", &__text_end);
  log_i("end is: 0x%p", end);
  log_i("PAGE_KERNEL_ROX is: 0x%llx", PAGE_KERNEL_ROX);
  log_i("page_kernel_rox is: 0x%llx", page_kernel_rox);
  log_i("pk_rox is: 0x%llx", pk_rox);
  // todo: non standard
  //  u64 x = *(u64 *)&page_kernel_rox;
  create_l0_mapping((l0_page_table_entry_t *)&l0_page_table, start, start, end - start, pk_rox, early_pgtable_alloc, 0);
  log_i("text start => end: 0x%llx => 0x%llx", start, end);
  // todo: map rodata section, data section

  /*map memory*/
  start = PAGE_ALIGN((unsigned long)&__text_end);
  end   = TOTAL_MEMORY;
  log_i("memory start => end: 0x%llx => 0x%llx", start, end);
  log_i("PAGE_KERNEL is: 0x%llx", PAGE_KERNEL);
  create_l0_mapping((l0_page_table_entry_t *)&l0_page_table, start, start, end - start, PAGE_KERNEL,
                    early_pgtable_alloc, 0);
}

static void create_mmio_mapping(void) {
  //  __create_pgd_mapping((pgd_t *)idmap_pg_dir, PBASE, PBASE,
  //                       DEVICE_SIZE, PROT_DEVICE_nGnRnE,
  //                       early_pgtable_alloc,
  //                       0);
  // NOTE: DEVICE_SIZE must be big enough to map all PERIPHERAL_BASE and LOCAL_PERIPHERALS_BASE
  create_l0_mapping((l0_page_table_entry_t *)&l0_page_table, 0x3F000000, 0x3F000000,
                    /*DEVICE_SIZE*/ 0x10000000, PROT_DEVICE_nGnRnE, early_pgtable_alloc, 0);
}

static void cpu_init(void) {
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

static int enable_mmu(void) {
  u64 tmp    = read_sysreg(ID_AA64MMFR0_EL1);
  u64 tgran4 = (tmp >> ID_AA64MMFR0_TGRAN4_SHIFT) & 0xf;
  log_i("tgran4 is: %llu", tgran4);
  if (tgran4 != ID_AA64MMFR0_TGRAN4_SUPPORTED) {
    return -1;
  }

  write_sysreg(&l0_page_table, ttbr0_el1);
  isb();

  write_sysreg(SCTLR_ELx_M, sctlr_el1);
  isb();
  asm("ic iallu");
  dsb(nsh);
  isb();

  return 0;
}

void paging_init(void) {
  log_i("l0_page_table is: 0x%p", &l0_page_table);
  // NOTE: PAGE_SIZE must be same with the value defined in the linker script
  memset(&l0_page_table, 0, PAGE_SIZE);
  create_identical_mapping();
  create_mmio_mapping();
  cpu_init();
  enable_mmu();

  log_i("enable mmu done\n");
}
