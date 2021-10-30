//
// Created by shifu on 2021/10/14.
//

#include "aarch64/kernel_space.hh"
#include "aarch64/mmu.hh"
#include "libcxx/macros.hh"
#include "libcxx/types.hh"
#include "libcxx/unit.hh"

[[gnu::used]] u64 _kernel_entry_paddr  = 0;
[[gnu::used]] u64 _arch_boot_el        = 0;
[[gnu::used]] u64 _kernel_base_phys    = 0;

constexpr auto ARCH_DEFAULT_STACK_SIZE = 8_KiB;
constexpr auto ARCH_PHYSIC_MAP_SIZE    = 1UL << 39; // (1<<39)/1024/1024/1024=512GB
static_assert(ARCH_PHYSIC_MAP_SIZE == 512_GiB);
constexpr auto MMU_PTE_KERNEL_DATA_FLAGS    = 0x60'0000'0000'0708UL;
constexpr auto MMU_PAGE_TABLE_ENTRIES_IDENT = 512;
constexpr auto ZX_TLS_STACK_GUARD_OFFSET    = -0x10;

extern_C [[gnu::used]] [[gnu::naked]] void _start() {
  asm volatile(
      R"(
.include "aarch64/asm_macros.hh"


/*
 * Register use:
 *  x0-x3   Arguments
 *  x9-x14  Scratch
 *  x21-x28 Globals
 */
tmp                     .req x9
tmp2                    .req x10
wtmp2                   .req w10
tmp3                    .req x11
tmp4                    .req x12
tmp5                    .req x13

percpu_ptr              .req x15

cpuid                   .req x21
page_table0             .req x22
page_table1             .req x23
kernel_vaddr            .req x24
//handoff_paddr           .req x25

.section .text.boot
_text_boot:
  /* Save the Boot info for the primary CPU only */
  mrs     cpuid, mpidr_el1
  ubfx    cpuid, cpuid, #0, #15 /* mask Aff0 and Aff1 fields */
  cbnz    cpuid, .Lno_save_bootinfo

  // Save the x0 argument in a register that won't be clobbered.
//    mov     handoff_paddr, x0

  /* save entry point physical address in _kernel_entry_paddr */
  adrp    tmp, _kernel_entry_paddr
  adr     tmp2, _start
  str     tmp2, [tmp, #:lo12:_kernel_entry_paddr]
  adrp    tmp2, _arch_boot_el
  mrs     x2, CurrentEL
  str     x2, [tmp2, #:lo12:_arch_boot_el]

.Lno_save_bootinfo:
  /* if we entered at a higher EL than 1, drop to EL1 */
  bl      arm64_elX_to_el1

  /* enable caches so atomics and spinlocks work */
  mrs     tmp, sctlr_el1
  orr     tmp, tmp, #(1<<12) /* Enable icache */
  orr     tmp, tmp, #(1<<2)  /* Enable dcache/ucache */
  msr     sctlr_el1, tmp

  // This can be any arbitrary (page-aligned) address >= KERNEL_SPACE_START.
  adr_global  tmp, kernel_relocated_base
  ldr     kernel_vaddr, [tmp]

  // Load the base of the translation tables.
  adr_global page_table0, translation_table_trampoline
  adr_global page_table1, arm64_kernel_translation_table

  // Send secondary cpus over to a waiting spot for the primary to finish.
  cbnz    cpuid, .Lmmu_enable_secondary

/* clear out the kernel's bss using current physical location */
/* NOTE: Relies on __bss_start and __kernel_end being 16 byte aligned */
.Ldo_bss:
  adr_global tmp, __bss_start
  adr_global tmp2, __bss_end
  sub     tmp2, tmp2, tmp
  cbz     tmp2, .Lbss_loop_done
.Lbss_loop:
  sub     tmp2, tmp2, #64
  stp     xzr, xzr, [tmp], #64
  cbnz    tmp2, .Lbss_loop
.Lbss_loop_done:

  /* set up a functional stack pointer before mmu are enabled */
  adr_global tmp, boot_cpu_kstack_end
  mov     sp, tmp

  /* make sure the boot allocator is given a chance to figure out where we are loaded in physical memory. */
  bl      boot_alloc_init

  // todo: maybe unimportant code
  /* save the physical address the kernel is loaded at */
  adr_global x0, __kernel_start
  adr_global x1, _kernel_base_phys
  str     x0, [x1] // _kernel_base_phys=__kernel_start

  /* clear out the kernel translation table */
  mov     tmp, #0
// for(int tmp = 0;tmp < MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP;tmp++) { arm64_kernel_translation_table[tmp] = 0; }
.Lclear_top_page_table_loop:
  str     xzr, [page_table1, tmp, lsl #3]
  add     tmp, tmp, #1
  cmp     tmp, %[MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP] // cmp tmp, #512
  bne     .Lclear_top_page_table_loop


  /* map a large run of physical memory at the base of the kernel's address space
   * TODO: Only map the arenas. */
  /* void arm64_boot_map(pte_t* kernel_table0, vaddr_t vaddr, paddr_t paddr, size_t len, pte_t flags); */
  mov     x0, page_table1
  mov     x1, %[KERNEL_SPACE_START]
  mov     x2, 0
  mov     x3, %[ARCH_PHYSIC_MAP_SIZE] // 512GB
  movlit  x4, %[MMU_PTE_KERNEL_DATA_FLAGS] // TODO: why use movlit, could use ldr???
  bl      arm64_boot_map
  cbz     x0, 1f
  // if mmu map failed
  bl      on_boot_map_failed

1:
  /* map the kernel to a fixed address */
  /* note: mapping the kernel here with full rwx, this will get locked down later in vm initialization; */
  mov     x0, page_table1
  mov     x1, kernel_vaddr // kernel_vaddr==kernel_relocated_base
  adr_global x2, __kernel_start
  adr_global x3, __kernel_end
  sub     x3, x3, x2
  mov     x4, %[MMU_PTE_KERNEL_RWX_FLAGS]
  bl      arm64_boot_map
  cbz     x0, 2f
  // if mmu map failed
  bl      on_boot_map_failed

2:
    /* Prepare translate_table_trampoline page table.
     * this will identity map the 1GB page holding the physical address of this code.
     * Used to temporarily help us get switched to the upper virtual address. */
    /* Zero translate_table_trampoline translation tables */
//todo: validate!!!
    mov     tmp, #0
.Lclear_translate_table_trampoline:
    str     xzr, [page_table0, tmp, lsl#3]
    add     tmp, tmp, #1
    cmp     tmp, %[MMU_PAGE_TABLE_ENTRIES_IDENT]
    blt     .Lclear_translate_table_trampoline

    /* Setup mapping at phys -> phys */
    adr     tmp, .Lmmu_on_pc
    lsr     tmp, tmp, %[MMU_IDENT_TOP_SHIFT]    /* tmp = paddr index */ // tmp >> 30
    movlit  tmp2, %[MMU_PTE_IDENT_FLAGS]
    // ADD R0, R2, R3,LSL #1 ; R0 = R2 + (R3 << 1)
    add     tmp2, tmp2, tmp, lsl %[MMU_IDENT_TOP_SHIFT]  /* tmp2 = pt entry */ // tmp2=tmp2+(tmp<<30)
    str     tmp2, [page_table0, tmp, lsl #3]  /* translate_table_trampoline[paddr index] = pt entry */

    /* mark page tables as set up, so secondary cpus can fall through */
    adr_global tmp, page_tables_not_ready
    str     wzr, [tmp]

    /* make sure it's flushed */
    dc      cvac, tmp
    dmb     sy

    b       .Lpage_tables_ready

.Lmmu_enable_secondary:
  adr_global tmp, page_tables_not_ready
  /* trap any secondary cpus until the primary has set up the page tables */
.Lpage_tables_not_ready:
  ldr     wtmp2, [tmp]
  cbnz    wtmp2, .Lpage_tables_not_ready
.Lpage_tables_ready:

    /* set up the mmu */

    /* Invalidate the entire TLB */
    tlbi    vmalle1is
    dsb     sy
    isb

    /* Initialize Memory Attribute Indirection Register */
    movlit  tmp, %[MMU_MAIR_VAL]
    msr     mair_el1, tmp

    /* Initialize TCR_EL1 */
    /* set cacheable attributes on translation walk */
    /* (SMP extensions) non-shareable, inner write-back write-allocate */
    /* both aspaces active, current ASID in TTBR1 */
    movlit  tmp, %[MMU_TCR_FLAGS_IDENT]
    msr     tcr_el1, tmp
    isb

    /* Write the ttbrs with phys addr of the translation table */
    msr     ttbr0_el1, page_table0
    /* Or in 0x1 (GLOBAL_ASID) bits. Keep in sync with mmu.h  */
    orr     tmp, page_table1, #(0x1 << 48)
    msr     ttbr1_el1, tmp
    isb

    /* Read SCTLR */
    mrs     tmp, sctlr_el1

    /* Turn on the MMU */
    orr     tmp, tmp, #(1<<0)

    /* Write back SCTLR */
    msr     sctlr_el1, tmp
.Lmmu_on_pc:
    isb

    // Map our current physical PC to the virtual PC and jump there.
    // PC = next_PC - __kernel_start + kernel_vaddr
    adr     tmp, .Lmmu_on_vaddr
    adr_global tmp2, __kernel_start
    sub     tmp, tmp, tmp2
    add     tmp, tmp, kernel_vaddr
    // jump to kernel space
    // when gdb debugging, should run gdb command: `add-symbol-file kernel.elf 0xffffffff00000000` to load the kernel image to kernel space
    br      tmp

.Lmmu_on_vaddr:
    /* Disable trampoline page-table in ttbr0 */
    movlit  tmp, %[MMU_TCR_FLAGS_KERNEL]
    msr     tcr_el1, tmp
    isb

    /* Invalidate the entire TLB */
    tlbi    vmalle1
    dsb     sy
    isb

    cbnz    cpuid, .Lsecondary_boot

    // set up the boot stack for real
    adr_global tmp, boot_cpu_kstack_end
    mov     sp, tmp

    // Set the thread pointer early so compiler-generated references
    // to the stack-guard and unsafe-sp slots work.  This is not a
    // real 'struct thread' yet, just a pointer to (past, actually)
    // the two slots used by the ABI known to the compiler. This avoids
    // having to compile-time disable safe-stack and stack-protector
    // code generation features for all the C code in the bootstrap
    // path, which (unlike on x86, e.g.) is enough to get annoying.
    adr_global tmp, boot_cpu_fake_thread_pointer_location
    msr     tpidr_el1, tmp
#if __has_feature(shadow_call_stack)
    // The shadow call stack grows up.
//    adr_global shadow_call_sp, boot_cpu_shadow_call_kstack
#endif

    // set the per cpu pointer for cpu 0
    adr_global percpu_ptr, arm64_percpu_array

    // Choose a good (ideally random) stack-guard value as early as possible.
    bl      choose_stack_guard
    mrs     tmp, tpidr_el1
    str     x0, [tmp, %[ZX_TLS_STACK_GUARD_OFFSET]]
    // Don't leak the value to other code.
    mov     x0, xzr

    // Collect the time stamp of entering "normal" C++ code in virtual space.
//    sample_ticks
//    store_ticks kernel_virtual_entry_ticks

    bl init_exception_vector_table
    bl  kernel_main
    // kernel_main should not return, if returned unexpected, stops here
    b   .

.Lsecondary_boot:
    bl      arm64_get_secondary_sp
    cbz     x0, .Lunsupported_cpu_trap
    mov     sp, x0
    msr     tpidr_el1, x1
#if __has_feature(shadow_call_stack)
//    mov     shadow_call_sp, x2
#endif

    bl      arm64_secondary_entry

.Lunsupported_cpu_trap:
    wfi
    b       .Lunsupported_cpu_trap

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DATA page_tables_not_ready
  .long       1
END_DATA page_tables_not_ready

.balign 8
  LOCAL_DATA boot_cpu_fake_arch_thread
  .quad 0xdeadbeef1ee2d00d // stack_guard
//.if __has_feature(safe_stack)
//  .quad boot_cpu_unsafe_kstack_end
//.else
  .quad 0
//.endif
LOCAL_DATA boot_cpu_fake_thread_pointer_location
END_DATA boot_cpu_fake_arch_thread


.bss
LOCAL_DATA boot_cpu_kstack
  .skip %[ARCH_DEFAULT_STACK_SIZE] // 8KiB
  .balign 16
LOCAL_DATA boot_cpu_kstack_end
END_DATA boot_cpu_kstack


.section .bss.prebss.translation_table, "aw", @nobits
.align 3 + %[MMU_IDENT_PAGE_TABLE_ENTRIES_TOP_SHIFT] // 3+9=12
DATA translation_table_trampoline
  .skip 8 * %[MMU_IDENT_PAGE_TABLE_ENTRIES_TOP] // 8*512=4096
END_DATA translation_table_trampoline
)"
      :
      : [MMU_IDENT_PAGE_TABLE_ENTRIES_TOP_SHIFT] "i"(MMU_IDENT_PAGE_TABLE_ENTRIES_TOP_SHIFT),
        [MMU_IDENT_PAGE_TABLE_ENTRIES_TOP] "i"(MMU_IDENT_PAGE_TABLE_ENTRIES_TOP),
        [ARCH_DEFAULT_STACK_SIZE] "i"(ARCH_DEFAULT_STACK_SIZE),
        [MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP] "i"(MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP),
        [KERNEL_SPACE_START] "i"(KERNEL_SPACE_START), [ARCH_PHYSIC_MAP_SIZE] "i"(ARCH_PHYSIC_MAP_SIZE),
        [MMU_PTE_KERNEL_DATA_FLAGS] "i"(MMU_PTE_KERNEL_DATA_FLAGS),
        [MMU_PTE_KERNEL_RWX_FLAGS] "i"(MMU_PTE_KERNEL_RWX_FLAGS),
        [MMU_PAGE_TABLE_ENTRIES_IDENT] "i"(MMU_PAGE_TABLE_ENTRIES_IDENT),
        [MMU_IDENT_TOP_SHIFT] "i"(MMU_IDENT_TOP_SHIFT()), [MMU_PTE_IDENT_FLAGS] "i"(MMU_PTE_IDENT_FLAGS),
        [MMU_MAIR_VAL] "i"(MMU_MAIR_VAL), [MMU_TCR_FLAGS_IDENT] "i"(MMU_TCR_FLAGS_IDENT),
        [MMU_TCR_FLAGS_KERNEL] "i"(MMU_TCR_FLAGS_KERNEL), [ZX_TLS_STACK_GUARD_OFFSET] "i"(ZX_TLS_STACK_GUARD_OFFSET)

      : "cc", "memory"); // todo: clobber should be set
}
