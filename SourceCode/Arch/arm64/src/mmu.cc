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
static L0PageTable L0_PAGE_TABLE_ADDRESS __attribute__((aligned(PAGE_SIZE)));

/// pointer to function to allocate page table
using FuncAllocatePage = u64 (*)();

static u64 locateL3EntryOffset(u64 virtualAddress) { return (virtualAddress >> PAGE_SHIFT) & (PTRS_PER_PTE - 1); }
static void l3Mapping(L2PageTableEntry *l2PageTableEntry, u64 virtualAddress, u64 end, u64 physicalAddress,
                      u64 property, FuncAllocatePage allocPage, u64 flags) {
  //  l3_page_table_entry *l3_page_table_entry = nullptr;

  if (*l2PageTableEntry == 0U) {
    u64 l3PhysicAddress = allocPage();
    //    set_pmd(pmdp, __pmd(pte_phys | PMD_TYPE_TABLE));
    *l2PageTableEntry = l3PhysicAddress | PMD_TYPE_TABLE;
    dsb(ishst);
  }

  /// [20:12]
  L3PageTableEntry *l3PageTableEntry = reinterpret_cast<L3PageTableEntry *>(
      ((*l2PageTableEntry & 0xffff'ffff'f000) + ((virtualAddress >> 12) & 0x1ff) * sizeof(L3PageTableEntry)));
  //  ptep = pte_offset_phys(pmdp, addr);
  log_i("pte index: 0x%llx", locateL3EntryOffset(virtualAddress));
  do {
    //    set_pte(ptep, pfn_pte(phys >> PAGE_SHIFT, prot));
    //    ptep->pte = (((phys >> 12) << 12) | (prot));
    *l3PageTableEntry = (physicalAddress & 0xFFFF'FFFF'FFFF'F000) | property;
    dsb(ishst);

    physicalAddress += PAGE_SIZE;
    l3PageTableEntry++;
    virtualAddress += PAGE_SIZE;
  } while (virtualAddress != end);
}

static void l2SetSection(L2PageTableEntry *l2PageTableEntry, u64 physicAddress, u64 property) {
  u64 sectionProperty = PMD_TYPE_SECT | makeSectionProperty(property);

  //  pmd_t new_pmd = pfn_pmd(phys >> PMD_SHIFT, sect_prot);
  L2PageTableEntry newL2PageTableEntry = ((physicAddress >> PMD_SHIFT) << PMD_SHIFT) | (sectionProperty);

  //  set_pmd(pmdp, new_pmd);
  *l2PageTableEntry = newL2PageTableEntry;

  dsb(ishst);
}
static u64 l2AddressEnd(u64 address, u64 end) {
  u64 boundary = (address + (1UL << 21)) & (~((1UL << 21) - 1));
  return boundary - 1 < end - 1 ? boundary : end;
}

static u64 locateL2EntryOffset(u64 address) { return ((address) >> PMD_SHIFT & (PTRS_PER_PMD - 1)); }
static void l2Mapping(L1PageTableEntry *l1PageTableEntry, u64 virtualAddress, u64 end, u64 physicalAddress,
                      u64 property, FuncAllocatePage allocPage, u64 flags) {
  if (*l1PageTableEntry == 0U) {
    u64 l2PhysicAddress = allocPage();
    //    set_pud(pudp, __pud(pmd_phys | PUD_TYPE_TABLE));
    *l1PageTableEntry = l2PhysicAddress | PUD_TYPE_TABLE;
    dsb(ishst);
  }

  /// [29:21]
  L2PageTableEntry *l2PageTableEntry = reinterpret_cast<L2PageTableEntry *>(
      (*l1PageTableEntry & 0xffff'ffff'f000) + (((virtualAddress >> 21) & 0x1ff) * sizeof(l2PageTableEntry)));
  //  pmd_t *pmdp = pmd_offset_phys(pudp, addr);
  log_i("pmdp: %p, pmd index 0x%llx", l2PageTableEntry, locateL2EntryOffset(virtualAddress));
  u64 next = 0;
  do {
    //    next = pmd_addr_end(addr, end);
    next = l2AddressEnd(virtualAddress, end);

    if (((virtualAddress | next | physicalAddress) & ~SECTION_MASK) == 0 && (flags & NO_BLOCK_MAPPINGS) == 0) {
      l2SetSection(l2PageTableEntry, physicalAddress, property);
    } else {
      l3Mapping(l2PageTableEntry, virtualAddress, next, physicalAddress, property, allocPage, flags);
    }

    physicalAddress += next - virtualAddress;
    l2PageTableEntry++;
    virtualAddress = next;
  } while (virtualAddress != end);
}
static u64 l1AddressEnd(u64 virtualAddress, u64 end) {
  u64 boundary = ((virtualAddress) + (1UL << 30)) & (~((1UL << 30) - 1));
  return boundary - 1 < end - 1 ? boundary : end;
}

static u64 locateL1EntryOffset(u64 virtualAddress) { return ((virtualAddress) >> PUD_SHIFT & (PTRS_PER_PUD - 1)); }
static void l1Mapping(L0PageTableEntry *l0PageTableEntry, u64 virtualAddress, u64 end, u64 physicalAddress,
                      u64 property, FuncAllocatePage allocPage, u64 flags) {
  if (*l0PageTableEntry == 0U) {
    u64 l1PhysicAddress = allocPage();
    log_i("pud_phys is: 0x%llx", l1PhysicAddress);

    *l0PageTableEntry = l1PhysicAddress | PUD_TYPE_TABLE;
    dsb(ishst);
    log_i("pgdp is: 0x%llx", *l0PageTableEntry);
  }

  /// [38:30]
  L1PageTableEntry *l1PageTableEntry = reinterpret_cast<L1PageTableEntry *>(*l0PageTableEntry & 0xffff'ffff'f000) +
                                       ((virtualAddress >> 30) & 0x1ff) * sizeof(L1PageTableEntry);
  //  pud_t *pudp = pud_offset_phys(pgdp, addr);
  log_i("pudp index: 0x%llx", locateL1EntryOffset(virtualAddress));
  u64 next = 0;
  do {
    //    next = pud_addr_end(virtual_address, end);
    next = l1AddressEnd(virtualAddress, end);
    l2Mapping(l1PageTableEntry, virtualAddress, next, physicalAddress, property, allocPage, flags);
    physicalAddress += next - virtualAddress;
    l1PageTableEntry++;
    virtualAddress = next;
  } while (virtualAddress != end);
}

static inline u64 l0PageTableIndex(u64 virtualAddress) {
  log_d("address is 0x%llx", virtualAddress);
  //  u64 offset = (address >> 39) & (1 << 8);
  /// virtual address offset in l0
  //  u64 offset = (virtual_address >> 39) & (0b1'1111'1111);
  /// [47:39] bits
  // TODO: confirm 0x0000'FF80'0000'0000
  u64 offset = virtualAddress & 0x0000'FF80'0000'0000;
  log_d("l0 page table offset is: 0x%llx", offset);
  return offset;
}

static inline L0PageTableEntry *locateL0EntryOffset(L0PageTableEntry *l0PageTableAddress, u64 virtualAddress) {
  return (l0PageTableAddress + l0PageTableIndex(virtualAddress));
}
static u64 l0AddressEnd(u64 virtualAddress, u64 end) {
  u64 boundary = (virtualAddress + (1UL << 39)) & (~((1UL << 39) - 1));
  return boundary - 1 < end - 1 ? boundary : end;
};

static void createL0Mapping(L0PageTableEntry *l0PageTableAddress, u64 physicalAddress, u64 virtualAddress, u64 size,
                            u64 property, FuncAllocatePage allocatePage, u64 flags) {
  log_i("prot is: 0x%llx", property);
  L0PageTableEntry *l0PageTableEntry = locateL0EntryOffset(l0PageTableAddress, virtualAddress);

  log_i("phys is: 0x%llx", physicalAddress);
  //  phys &= PAGE_MASK;
  physicalAddress &= 0xffff'ffff'ffff'f000;
  log_i("phys is: 0x%llx", physicalAddress);
  log_i("virt is: 0x%llx", virtualAddress);
  //  u64 virtual_address = virt & PAGE_MASK;
  u64 maskedVirtualAddress = virtualAddress & 0xffff'ffff'ffff'f000;
  log_i("addr is: 0x%llx", maskedVirtualAddress);
  log_i("virt + size is: 0x%llx", virtualAddress + size);
  u64 end = PAGE_ALIGN(virtualAddress + size);
  log_i("end is: 0x%llx", end);

  u64 next = 0;
  do {
    log_i("l0 do while");
    //    next = pgd_addr_end(virtual_address, end);
    next = l0AddressEnd(maskedVirtualAddress, end);
    l1Mapping(l0PageTableEntry, maskedVirtualAddress, next, physicalAddress, property, allocatePage, flags);
    physicalAddress += next - maskedVirtualAddress;
    l0PageTableEntry++;
    maskedVirtualAddress = next;
  } while (maskedVirtualAddress != end);
}

extern u64 getFreePage();
static u64 earlyAllocatePageTable() {
  u64 phys = getFreePage();
  memset((void *)phys, 0, PAGE_SIZE);

  return phys;
}

FuncAllocatePage DEFAULT_FUNC_ALLOCATE_PAGE = &earlyAllocatePageTable;

extern u64 __text_start;
extern u64 __text_end;

static void createIdenticalMapping() {
  /*map text*/
  u64 start = reinterpret_cast<u64>(&__text_start);
  u64 end   = reinterpret_cast<u64>(&__text_end);
  log_i("PAGE_KERNEL_ROX is: 0x%llx", PAGE_KERNEL_ROX);
  log_i("page_kernel_rox is: 0x%llx", PAGE_KERNEL_ROX);
  log_i("pk_rox is: 0x%llx", PAGE_KERNEL_ROX);
  createL0Mapping(reinterpret_cast<L0PageTableEntry *>(&L0_PAGE_TABLE_ADDRESS), start, start, end - start,
                  PAGE_KERNEL_ROX, DEFAULT_FUNC_ALLOCATE_PAGE, 0);
  log_i("text start => end: 0x%llx => 0x%llx", start, end);
  // todo: map rodata section, data section

  /*map memory*/
  start = PAGE_ALIGN(end);
  end   = TOTAL_MEMORY;
  log_i("memory start => end: 0x%llx => 0x%llx", start, end);
  log_i("PAGE_KERNEL is: 0x%llx", PAGE_KERNEL);
  createL0Mapping(reinterpret_cast<L0PageTableEntry *>(&L0_PAGE_TABLE_ADDRESS), start, start, end - start, PAGE_KERNEL,
                  DEFAULT_FUNC_ALLOCATE_PAGE, 0);
}

static void createMmioIdenticalMapping() {
  //  __create_pgd_mapping((pgd_t *)idmap_pg_dir, PBASE, PBASE,
  //                       DEVICE_SIZE, PROT_DEVICE_nGnRnE,
  //                       default_func_allocate_page,
  //                       0);
  // NOTE: DEVICE_SIZE must be big enough to map all PERIPHERAL_BASE and LOCAL_PERIPHERALS_BASE
  createL0Mapping(reinterpret_cast<L0PageTableEntry *>(&L0_PAGE_TABLE_ADDRESS), 0x3F00'0000, 0x3F00'0000,
                  /*DEVICE_SIZE*/ 0x10000000, PROT_DEVICE_nGnRnE, DEFAULT_FUNC_ALLOCATE_PAGE, 0);
}

static void cpuInit() {
  asm volatile("tlbi vmalle1");
  dsb(nsh);

  ARM64_WRITE_SYSTEM_REGISTER(cpacr_el1, 3UL << 20);
  ARM64_WRITE_SYSTEM_REGISTER(mdscr_el1, 1UL << 12);

  setupMairEl1();

  u64 tcr = TCR_TxSZ(VA_BITS) | TCR_TG_FLAGS;

  u64 tmp    = ARM64_READ_SYSTEM_REGISTER(ID_AA64MMFR0_EL1);
  u64 parang = tmp & 0xf;
  if (parang > ID_AA64MMFR0_PARANGE_48) {
    parang = ID_AA64MMFR0_PARANGE_48;
  }

  tcr |= parang << TCR_IPS_SHIFT;

  ARM64_WRITE_SYSTEM_REGISTER(tcr_el1, tcr);
}

static int enableMmu() {
  u64 tmp    = ARM64_READ_SYSTEM_REGISTER(ID_AA64MMFR0_EL1);
  u64 tgran4 = (tmp >> ID_AA64MMFR0_TGRAN4_SHIFT) & 0xf;
  log_i("tgran4 is: %llu", tgran4);
  if (tgran4 != ID_AA64MMFR0_TGRAN4_SUPPORTED) {
    log_e("tgran4 is not supported, tgran4 is: %ll", tgran4);
    return -1;
  }

  ARM64_WRITE_SYSTEM_REGISTER(ttbr0_el1, &L0_PAGE_TABLE_ADDRESS);
  isb();

  ARM64_WRITE_SYSTEM_REGISTER(sctlr_el1, SCTLR_ELx_M);
  isb();
  asm volatile("ic iallu");
  dsb(nsh);
  isb();

  return 0;
}

void pagingInit() {
  log_i("l0_page_table is: 0x%p", &L0_PAGE_TABLE_ADDRESS);
  // &l0_page_table_address is in bss section, it's already be cleaned before, there's no need to clean it again
  // memset(&&l0_page_table_address, 0, PAGE_SIZE);
  createIdenticalMapping();
  createMmioIdenticalMapping();

  cpuInit();
  enableMmu();

  log_i("enable mmu done");
}

void initMmu() { pagingInit(); }
