#include "aarch64/arch.hh" // for ARCH_DEFAULT_STACK_SIZE, ARCH_PHYSMAP_SIZE
#include "aarch64/mmu.hh"  // for MMU_IDENT_PAGE_TABLE_ENTRIES_TOP, MMU_IDE...
#include "aarch64/mp.hh"   // for CONF_SMP_MAX_CPUS
#include "kernel/space.hh" // for KERNEL_SPACE_BASE
#include "libcxx/attr.hh"  // for attr_maybe_unused, attr_naked, attr_noreturn

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
.macro set_cpu_sp
    adr_g   tmp, boot_cpu_kstack_end
    mov     tmp2, %[ARCH_DEFAULT_STACK_SIZE]
    mul     tmp2, cpuid, tmp2
    sub     tmp, tmp, tmp2
    mov     sp, tmp
.endm


/* This code is purely position-independent */
.section .text.boot
FUNCTION _start
    /* Save the Boot info for the primary CPU only */
    mrs     cpuid, mpidr_el1
    ubfx    cpuid, cpuid, #0, #15 /* mask Aff0 and Aff1 fields */
    cbnz    cpuid, .Lno_save_bootinfo

    // Save the x0 argument in a register that won't be clobbered.
    mov     handoff_paddr, x0

.Lno_save_bootinfo:
    /* if we entered at a higher EL than 1, drop to EL1 */
    bl      arm64_elX_to_el1

    /* enable caches so atomics and spinlocks work */
    mrs     tmp, sctlr_el1
    orr     tmp, tmp, #(1<<12) /* Enable icache */
    orr     tmp, tmp, #(1<<2)  /* Enable dcache/ucache */
    msr     sctlr_el1, tmp

    // This can be any arbitrary (page-aligned) address >= KERNEL_SPACE_BASE.
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
    set_cpu_sp

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
    mov     x1, %[KERNEL_SPACE_BASE] // 0xffff'0000'0000'0000
    mov     x2, 0
    mov     x3, %[ARCH_PHYSMAP_SIZE] // 1<<39=512G
    movlit  x4, %[MMU_PTE_KERNEL_DATA_FLAGS] // 0x60'0000'0000'0708
    bl      arm64_boot_map
    cmp     x0, #0
    bne     .

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
    cmp     x0, #0
    bne     .

    /* map platform peripherals */
    mov     x0, page_table1
    bl      arm64_boot_map_platform_peripherals
    cmp     x0, #0
    bne     .

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
    set_cpu_sp

    mov x0, handoff_paddr
    bl  kernel_main
    b   .Lunsupported_cpu_trap

.Lsecondary_boot:
    set_cpu_sp
    bl      secondary_entry

.Lunsupported_cpu_trap:
    wfi
    b       .Lunsupported_cpu_trap

END_FUNCTION _start


.ltorg

// These are logically .bss (uninitialized data).  But they're set before
// clearing the .bss, so put them in .data so they don't get zeroed.
.data
    .balign 64
DATA page_tables_not_ready
    .long       1
END_DATA page_tables_not_ready

.bss
LOCAL_DATA boot_cpu_kstack
    // sp for per cpu: boot_cpu_kstack_end-(cpuid*ARCH_DEFAULT_STACK_SIZE)
    .skip %[ARCH_DEFAULT_STACK_SIZE] * %[CONF_SMP_MAX_CPUS] // ARCH_DEFAULT_STACK_SIZE=8192
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
        [KERNEL_SPACE_BASE] "i"(KERNEL_SPACE_BASE),                                           // 0xffff'0000'0000'0000UL
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
        [ARCH_DEFAULT_STACK_SIZE] "i"(ARCH_DEFAULT_STACK_SIZE),                               // 8192
        [CONF_SMP_MAX_CPUS] "I"(CONF_SMP_MAX_CPUS)                                            // max cpu count

      : "cc", "memory");
}
