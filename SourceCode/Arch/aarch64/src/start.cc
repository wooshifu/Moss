#include "aarch64/arch.hh"
#include "aarch64/mmu.hh"
#include "kernel/space.hh"
#include "libcxx/attr.hh"

attr_optnone attr_naked attr_noreturn attr_maybe_unused void __start() {
  // all asm code must be position independent code(pic) code
  asm volatile(
      R"(
.include "aarch64/asm.hh"
.include "libstd/asm.hh"

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

cpuid                   .req x21
page_table0             .req x22
page_table1             .req x23
kernel_vaddr            .req x24
handoff_paddr           .req x25

// Collect timestamp in tmp, tmp2.  Also clobbers tmp3-5.
.macro sample_ticks
    mrs     tmp, cntpct_el0
    mrs     tmp2, cntvct_el0

    // Workaround for Cortex-A73 erratum 858921.
    mrs     tmp3, cntpct_el0
    mrs     tmp4, cntvct_el0
    eor     tmp5, tmp, tmp3
    tst     tmp5, #(1 << 32)
    csel    tmp, tmp, tmp3, eq
    eor     tmp5, tmp2, tmp4
    tst     tmp5, #(1 << 32)
    csel    tmp2, tmp2, tmp4, eq
.endm

// Store sample_ticks results in a uint64_t[2] location. Clobbers tmp3.
.macro store_ticks symbol
    // There is no reloc like :lo12: that works for stp's scaled immediate,
    // so the add after the adrp can't be folded into the store like with str.
    adr_g   tmp3, \symbol
    stp     tmp, tmp2, [tmp3]
.endm


/* This code is purely position-independent */
.section .text.boot
FUNCTION _start
    // As early as possible collect the time stamp.
    sample_ticks

    /* Save the Boot info for the primary CPU only */
    mrs     cpuid, mpidr_el1
    ubfx    cpuid, cpuid, #0, #15 /* mask Aff0 and Aff1 fields */
    cbnz    cpuid, .Lno_save_bootinfo

    // Record the entry time stamp.
    store_ticks kernel_entry_ticks

    // Save the x0 argument in a register that won't be clobbered.
    mov     handoff_paddr, x0

    /* save entry point physical address in kernel_entry_paddr */
    adrp    tmp, kernel_entry_paddr
    adr     tmp2, _start
    str     tmp2, [tmp, #:lo12:kernel_entry_paddr]
    adrp    tmp2, arch_boot_el
    mrs     x2, CurrentEL
    str     x2, [tmp2, #:lo12:arch_boot_el]

.Lno_save_bootinfo:
    /* if we entered at a higher EL than 1, drop to EL1 */
    bl      arm64_elX_to_el1

    /* enable caches so atomics and spinlocks work */
    mrs     tmp, sctlr_el1
    orr     tmp, tmp, #(1<<12) /* Enable icache */
    orr     tmp, tmp, #(1<<2)  /* Enable dcache/ucache */
    msr     sctlr_el1, tmp

    // This can be any arbitrary (page-aligned) address >= KERNEL_ASPACE_BASE.
    adr_g   tmp, kernel_relocated_base
    ldr     kernel_vaddr, [tmp]

    // Load the base of the translation tables.
    adr_g   page_table0, tt_trampoline
    adr_g   page_table1, arm64_kernel_translation_table

    // Send secondary cpus over to a waiting spot for the primary to finish.
    cbnz    cpuid, .Lmmu_enable_secondary

    /* clear out the kernel's bss using current physical location */
    /* NOTE: Relies on __bss_start and _end being 16 byte aligned */
.Ldo_bss:
    adr_g   tmp, __bss_start
    adr_g   tmp2, _end
    sub     tmp2, tmp2, tmp
    cbz     tmp2, .Lbss_loop_done
.Lbss_loop:
    sub     tmp2, tmp2, #16
    stp     xzr, xzr, [tmp], #16
    cbnz    tmp2, .Lbss_loop
.Lbss_loop_done:

    /* set up a functional stack pointer */
    adr_g   tmp, boot_cpu_kstack_end
    mov     sp, tmp

    /* make sure the boot allocator is given a chance to figure out where we are loaded in physical memory. */
    adr_g   tmp, boot_alloc_start
    adr_g   tmp2, _end
    str     tmp2, [tmp]
    adr_g   tmp, boot_alloc_end
    str     tmp2, [tmp]

    /* save the physical address the kernel is loaded at */
    adr_g   x0, __code_start
    adr_g   x1, kernel_base_phys
    str     x0, [x1]

    /* Save a copy of the physical address of the kernel page table */
    adr_g   tmp, arm64_kernel_translation_table_phys
    str     page_table1, [tmp]

    /* set up the mmu according to mmu_initial_mappings */

    /* clear out the kernel translation table */
    mov     tmp, #0
.Lclear_top_page_table_loop:
    str     xzr, [page_table1, tmp, lsl #3]
    add     tmp, tmp, #1
    cmp     tmp, #%[MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP] // 512
    bne     .Lclear_top_page_table_loop

    /* void arm64_boot_map(pte_t* kernel_table0, vaddr_t vaddr, paddr_t paddr, size_t len, pte_t flags); */

    /* map a large run of physical memory at the base of the kernel's address space
    /* from 0xffff'0000'0000'0000 to 0xffff'0080'0000'0000 */
    mov     x0, page_table1
    mov     x1, %[KERNEL_ASPACE_BASE] // 0xffff'0000'0000'0000
    mov     x2, 0
    mov     x3, %[ARCH_PHYSMAP_SIZE] // 1<<39=512G
    movlit  x4, %[MMU_PTE_KERNEL_DATA_FLAGS] // 0x60'0000'0000'0708
    bl      arm64_boot_map

    /* map the kernel to a fixed address */
    /* note: mapping the kernel here with full rwx, this will get locked down later in vm initialization; */
    /* from 0xffff'ffff'0000'0000 to 0xffff'ffff'0000'0000+(sizeof kernel.elf) */
    mov     x0, page_table1
    mov     x1, kernel_vaddr // kernel_relocated_base==0xffff'ffff'0000'0000
    adr_g   x2, __code_start
    adr_g   x3, _end
    sub     x3, x3, x2
    movlit  x4, %[MMU_PTE_KERNEL_RWX_FLAGS] // 0x40'0000'0000'0708
    bl      arm64_boot_map

    /* Prepare tt_trampoline page table.
     * this will identity map the 1GB page holding the physical address of this code.
     * Used to temporarily help us get switched to the upper virtual address. */

    /* Zero tt_trampoline translation tables */
    mov     tmp, #0
.Lclear_tt_trampoline:
    str     xzr, [page_table0, tmp, lsl#3]
    add     tmp, tmp, #1
    cmp     tmp, #%[MMU_IDENT_PAGE_TABLE_ENTRIES_TOP] // 512
    blt     .Lclear_tt_trampoline

    /* Setup mapping at phys -> phys */
    adr     tmp, .Lmmu_on_pc
    lsr     tmp, tmp, #%[MMU_IDENT_TOP_SHIFT] // 30   /* tmp = paddr index */
    movlit  tmp2, %[MMU_PTE_IDENT_FLAGS] // 0x40'0000'0000'0709
    add     tmp2, tmp2, tmp, lsl #%[MMU_IDENT_TOP_SHIFT] // 30 /* tmp2 = pt entry */

    str     tmp2, [page_table0, tmp, lsl #3]  /* tt_trampoline[paddr index] = pt entry */

    /* mark page tables as set up, so secondary cpus can fall through */
    adr_g   tmp, page_tables_not_ready
    str     wzr, [tmp]

    /* make sure it's flushed */
    dc      cvac, tmp
    dmb     sy

    b       .Lpage_tables_ready

.Lmmu_enable_secondary:
    adr_g   tmp, page_tables_not_ready
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
    movlit  tmp, %[MMU_MAIR_VAL] // 0x44ff'0400
    msr     mair_el1, tmp

    /* Initialize TCR_EL1 */
    /* set cacheable attributes on translation walk */
    /* (SMP extensions) non-shareable, inner write-back write-allocate */
    /* both aspaces active, current ASID in TTBR1 */
    movlit  tmp, %[MMU_TCR_FLAGS_IDENT] // 0x12'b550'3519
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
    // PC = next_PC - __code_start + kernel_vaddr
    adr     tmp, .Lmmu_on_vaddr
    adr_g   tmp2, __code_start
    sub     tmp, tmp, tmp2
    add     tmp, tmp, kernel_vaddr
    br      tmp

.Lmmu_on_vaddr:
    /* Disable trampoline page-table in ttbr0 */
    movlit  tmp, %[MMU_TCR_FLAGS_KERNEL] // 0x12'b550'3590
    msr     tcr_el1, tmp
    isb

    /* Invalidate the entire TLB */
    tlbi    vmalle1
    dsb     sy
    isb

    cbnz    cpuid, .Lsecondary_boot

    // set up the boot stack for real
    adr_g   tmp, boot_cpu_kstack_end
    mov     sp, tmp

    // Set the thread pointer early so compiler-generated references
    // to the stack-guard and unsafe-sp slots work.  This is not a
    // real 'struct thread' yet, just a pointer to (past, actually)
    // the two slots used by the ABI known to the compiler.  This avoids
    // having to compile-time disable safe-stack and stack-protector
    // code generation features for all the C code in the bootstrap
    // path, which (unlike on x86, e.g.) is enough to get annoying.
    adr_g   tmp, boot_cpu_fake_thread_pointer_location
    msr     tpidr_el1, tmp

    // set the per cpu pointer for cpu 0
    adr_g   percpu_ptr, arm64_percpu_array

    // Collect the time stamp of entering "normal" C++ code in virtual space.
    sample_ticks
    store_ticks kernel_virtual_entry_ticks

    mov x0, handoff_paddr
    bl  kernel_main
    b   .

.Lsecondary_boot:
    bl      arm64_get_secondary_sp // todo: arm64_get_secondary_sp currently not processed, arm64_get_secondary_sp currently will always return 0
    cbz     x0, .Lunsupported_cpu_trap
    mov     sp, x0
    msr     tpidr_el1, x1

    bl      arm64_secondary_entry

.Lunsupported_cpu_trap:
    wfe
    b       .Lunsupported_cpu_trap

END_FUNCTION _start


.ltorg

// These are logically .bss (uninitialized data).  But they're set before
// clearing the .bss, so put them in .data so they don't get zeroed.
.data
    .balign 64
DATA arch_boot_el
    .quad 0xdeadbeef00ff00ff
END_DATA arch_boot_el
DATA kernel_entry_paddr
    .quad -1
END_DATA kernel_entry_paddr
DATA kernel_entry_ticks
    .quad -1, -1
END_DATA kernel_entry_ticks
DATA page_tables_not_ready
    .long       1
END_DATA page_tables_not_ready

    .balign 8
LOCAL_DATA boot_cpu_fake_arch_thread
    .quad 0xdeadbeef1ee2d00d // stack_guard
    .quad 0
LOCAL_DATA boot_cpu_fake_thread_pointer_location
END_DATA boot_cpu_fake_arch_thread

.bss
LOCAL_DATA boot_cpu_kstack
    .skip %[ARCH_DEFAULT_STACK_SIZE] // 8192
    .balign 16
LOCAL_DATA boot_cpu_kstack_end
END_DATA boot_cpu_kstack

.section .bss.prebss.translation_table, "aw", @nobits
.align 3 + %[MMU_IDENT_PAGE_TABLE_ENTRIES_TOP_SHIFT] // 9
DATA tt_trampoline
    .skip 8 * %[MMU_IDENT_PAGE_TABLE_ENTRIES_TOP] // 512
END_DATA tt_trampoline

)"
      :
      : [MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP] "i"(MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP),           // 512
        [KERNEL_ASPACE_BASE] "i"(KERNEL_ASPACE_BASE),                                         // 0xffff'0000'0000'0000UL
        [MMU_PTE_KERNEL_RWX_FLAGS] "i"(MMU_PTE_KERNEL_RWX_FLAGS),                             // 0x40'0000'0000'0708
        [MMU_PTE_KERNEL_DATA_FLAGS] "i"(MMU_PTE_KERNEL_DATA_FLAGS),                           // 0x60'0000'0000'0708
        [ARCH_PHYSMAP_SIZE] "i"(ARCH_PHYSMAP_SIZE),                                           // 0x80'0000'0000=512G
        [MMU_IDENT_PAGE_TABLE_ENTRIES_TOP] "i"(MMU_IDENT_PAGE_TABLE_ENTRIES_TOP),             // 512
        [MMU_IDENT_PAGE_TABLE_ENTRIES_TOP_SHIFT] "i"(MMU_IDENT_PAGE_TABLE_ENTRIES_TOP_SHIFT), // 9
        [MMU_IDENT_TOP_SHIFT] "i"(MMU_IDENT_TOP_SHIFT),                                       // 30
        [MMU_PTE_IDENT_FLAGS] "i"(MMU_PTE_IDENT_FLAGS),                                       // 0x40'0000'0000'0709
        [MMU_MAIR_VAL] "i"(MMU_MAIR_VAL),                                                     // 0x44ff'0400
        [MMU_TCR_FLAGS_IDENT] "i"(MMU_TCR_FLAGS_IDENT),                                       // 0x12'b550'3519
        [MMU_TCR_FLAGS_KERNEL] "i"(MMU_TCR_FLAGS_KERNEL),                                     // 0x12'b550'3590
        [ARCH_DEFAULT_STACK_SIZE] "i"(ARCH_DEFAULT_STACK_SIZE)                                // 8192

      : "cc", "memory");
}
