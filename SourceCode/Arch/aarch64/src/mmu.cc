#include "aarch64/asm.hh"
#include "aarch64/barrier.hh"
#include "aarch64/linker.hh"
#include "aarch64/mair.hh"
#include "aarch64/page.hh"
#include "aarch64/page_property.hh"
#include "libcxx/log.hh"

/// the l0 page table
[[gnu::aligned(PAGE_SIZE)]] static L0PageTable l0_page_table_base_address;

/// &l0_page_table_base_address is in bss section, it's already be cleaned before, there's no need to clear it again.
/// but just in case, still clear it again
static void clear_l0_page_table() {
  log_d("address of l0 page table: 0x%p", &l0_page_table_base_address);
  for (auto& address : l0_page_table_base_address) {
    address = 0;
  }
}

/// pointer to function to allocate page table
using func_allocate_page = u64 (*)();

constexpr auto PAGE_TABLE_TYPE_TABLE = 0b11UL;

static void l3_mapping(L2PageTableEntry* l2_page_table_entry, u64 virtual_address, u64 end, u64 physical_address,
                       u64 property, func_allocate_page alloc_page, u64 flags) {
  if (*l2_page_table_entry == 0U) {
    u64 l3_physic_address = alloc_page();
    *l2_page_table_entry  = l3_physic_address | PAGE_TABLE_TYPE_TABLE;
    dsb(ishst);
  }

  /// [20:12]
  L3PageTableEntry* l3_page_table_entry = reinterpret_cast<L3PageTableEntry*>(
      ((*l2_page_table_entry & 0xffff'ffff'f000) + ((virtual_address >> 12) & 0x1ff) * sizeof(L3PageTableEntry)));
  do {
    *l3_page_table_entry = (physical_address & 0xFFFF'FFFF'FFFF'F000) | property;
    dsb(ishst);

    physical_address += PAGE_SIZE;
    l3_page_table_entry++;
    virtual_address += PAGE_SIZE;
  } while (virtual_address != end);
}

static void l2_set_section(L2PageTableEntry* l2_page_table_entry, u64 physic_address, u64 property) {
  u64 section_property                     = (1UL << 0) | make_section_property(property);
  L2PageTableEntry new_l2_page_table_entry = ((physic_address >> 21) << 21) | (section_property);
  *l2_page_table_entry                     = new_l2_page_table_entry;

  dsb(ishst);
}
static u64 l2_address_end(u64 addr, u64 end) {
  u64 boundary = (addr + (1UL << 21)) & (~((1UL << 21) - 1));
  return boundary - 1 < end - 1 ? boundary : end;
}

static void l2_mapping(L1PageTableEntry* l1_page_table_entry, u64 virtual_address, u64 end, u64 physical_address,
                       u64 property, func_allocate_page alloc_page, u64 flags) {
  if (*l1_page_table_entry == 0U) {
    u64 l2_physic_address = alloc_page();
    *l1_page_table_entry  = l2_physic_address | PAGE_TABLE_TYPE_TABLE;
    dsb(ishst);
  }

  /// [29:21]
  L2PageTableEntry* l2_page_table_entry = reinterpret_cast<L2PageTableEntry*>(
      (*l1_page_table_entry & 0xffff'ffff'f000) + (((virtual_address >> 21) & 0x1ff) * sizeof(l2_page_table_entry)));
  u64 next = 0;
  do {
    next = l2_address_end(virtual_address, end);

    if (((virtual_address | next | physical_address) & ((1 << 21) - 1)) == 0 and (flags & 1UL) == 0) {
      l2_set_section(l2_page_table_entry, physical_address, property);
    } else {
      l3_mapping(l2_page_table_entry, virtual_address, next, physical_address, property, alloc_page, flags);
    }

    physical_address += next - virtual_address;
    l2_page_table_entry++;
    virtual_address = next;
  } while (virtual_address != end);
}
static u64 l1_address_end(u64 virtual_address, u64 end) {
  u64 boundary = ((virtual_address) + (1UL << 30)) & (~((1UL << 30) - 1));
  return boundary - 1 < end - 1 ? boundary : end;
}

static void l1_mapping(L0PageTableEntry* l0_page_table_entry, u64 virtual_address, u64 end, u64 physical_address,
                       u64 property, func_allocate_page alloc_page, u64 flags) {
  if (*l0_page_table_entry == 0U) {
    u64 l1_physic_address = alloc_page();
    // Table:
    // 63   62-61  60   59     58-52    51-48    47-(m-1)  (m-1)-2   11
    // NST  APT    XNT  PXNT   Ignored  RES0     ADDR      Ignored   11
    // NST - NSTable. For secure memory accesses, determines type of next level. Otherwise ignored.
    // APT - APTable. Access permissions limit for next level lookup.
    // XNT - XNTable. XN limit for subsequent lookups.
    // PXNT - PXNTable. PXN limit for subsequent levels. SBZ for non-secure PL2 (hypervisor) level 1 translation tables.
    *l0_page_table_entry = l1_physic_address | PAGE_TABLE_TYPE_TABLE;
    dsb(ishst);
  }

  /// [38:30]
  L1PageTableEntry* l1_page_table_entry = reinterpret_cast<L1PageTableEntry*>(
      (*l0_page_table_entry & 0xffff'ffff'f000) + ((virtual_address >> 30) & 0x1ff) * sizeof(L1PageTableEntry));

  u64 next = 0;
  do {
    next = l1_address_end(virtual_address, end);
    l2_mapping(l1_page_table_entry, virtual_address, next, physical_address, property, alloc_page, flags);
    physical_address += next - virtual_address;
    l1_page_table_entry++;
    virtual_address = next;
  } while (virtual_address != end);
}

static inline u64 l0_page_table_index(u64 virtual_address) {
  /// virtual address offset in l0
  /// [47:39] bits
  // TODO: confirm 0x0000'FF80'0000'0000
  return virtual_address & 0x0000'FF80'0000'0000;
}

static inline L0PageTableEntry* locate_l0_entry_offset(L0PageTableEntry* l0_page_table_address, u64 virtual_addr) {
  return (l0_page_table_address + l0_page_table_index(virtual_addr));
}
static u64 l0_address_end(u64 virtual_address, u64 end) {
  u64 boundary = (virtual_address + (1UL << 39)) & (~((1UL << 39) - 1));
  return boundary - 1 < end - 1 ? boundary : end;
};

static void create_l0_mapping(L0PageTableEntry* l0_page_table_address, u64 physical_address, u64 virtual_address,
                              u64 size, u64 property, func_allocate_page alloc_page, u64 flags) {
  L0PageTableEntry* l0_page_table_entry = locate_l0_entry_offset(l0_page_table_address, virtual_address);

  physical_address &= 0xffff'ffff'ffff'f000;
  u64 end = PAGE_ALIGN(virtual_address + size);
  virtual_address &= 0xffff'ffff'ffff'f000;

  u64 next = 0;
  do {
    next = l0_address_end(virtual_address, end);
    l1_mapping(l0_page_table_entry, virtual_address, next, physical_address, property, alloc_page, flags);
    physical_address += next - virtual_address;
    l0_page_table_entry++;
    virtual_address = next;
  } while (virtual_address != end);
}

extern u64 get_free_page();
static u64 early_allocate_page_table() {
  log_d("early_allocate_page_table");
  u64 phys = get_free_page();
  memset(reinterpret_cast<void*>(phys), 0, PAGE_SIZE);

  return phys;
}
func_allocate_page default_func_allocate_page = &early_allocate_page_table;

static void create_identical_mapping() {
  // map text
  u64 text_start = get_text_start_address();
  u64 text_end   = get_text_end_address();
  u64 text_size  = get_text_size();
  log_d("text start => end: 0x%llx => 0x%llx, size: %lld", text_start, text_end, text_size);
  create_l0_mapping((L0PageTableEntry*)&l0_page_table_base_address, text_start, text_start, text_size, PAGE_KERNEL_ROX,
                    default_func_allocate_page, 0);

  // map memory
  u64 memory_start = PAGE_ALIGN(text_end);
  u64 memory_end   = TOTAL_MEMORY;
  u64 memory_size  = memory_end - memory_start;
  log_d("memory start => end: 0x%llx => 0x%llx, size: %lld", memory_start, memory_end, memory_size);
  create_l0_mapping(reinterpret_cast<L0PageTableEntry*>(&l0_page_table_base_address), memory_start, memory_start,
                    memory_size, PAGE_KERNEL, default_func_allocate_page, 0);
}

static void create_mmio_identical_mapping() {
  // NOTE: DEVICE_SIZE must be big enough to map all PERIPHERAL_BASE and LOCAL_PERIPHERALS_BASE
  create_l0_mapping(reinterpret_cast<L0PageTableEntry*>(&l0_page_table_base_address), 0x3F00'0000, 0x3F00'0000,
                    /*DEVICE_SIZE*/ 0x1000'0000, PROT_DEVICE_nGnRnE, default_func_allocate_page, 0);
}

static void cpu_init() {
  asm volatile("tlbi vmalle1");
  dsb(nsh);

  ARM64_WRITE_SYSREG(cpacr_el1, 3UL << 20);
  ARM64_WRITE_SYSREG(mdscr_el1, 1UL << 12);

  setup_mair_el1();

  u64 tcr = TCR_TxSZ(VA_BITS) | TCR_TG_FLAGS;

  u64 tmp    = ARM64_READ_SYSREG(ID_AA64MMFR0_EL1);
  u64 parang = tmp & 0xf;
  if (parang > ID_AA64MMFR0_PARANGE_48) {
    parang = ID_AA64MMFR0_PARANGE_48;
  }

  tcr |= parang << TCR_IPS_SHIFT;

  ARM64_WRITE_SYSREG(tcr_el1, tcr);
}

static int enable_mmu() {
  u64 tmp    = ARM64_READ_SYSREG(ID_AA64MMFR0_EL1);
  u64 tgran4 = (tmp >> ID_AA64MMFR0_TGRAN4_SHIFT) & 0xf;
  if (tgran4 != ID_AA64MMFR0_TGRAN4_SUPPORTED) {
    log_e("tgran4 is not supported, tgran4 is: %ll", tgran4);
    return -1;
  }

  ARM64_WRITE_SYSREG(ttbr0_el1, &l0_page_table_base_address);
  isb();

  ARM64_WRITE_SYSREG(sctlr_el1, 1UL << 0);
  isb();
  asm volatile("ic iallu");
  dsb(nsh);
  isb();

  return 0;
}

void init_mmu() {
  log_d("start to init mmu");

  clear_l0_page_table();
  create_identical_mapping();
  create_mmio_identical_mapping();
  cpu_init();
  enable_mmu();

  log_d("successfully init mmu");
}
