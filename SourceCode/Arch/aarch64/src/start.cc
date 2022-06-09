#include "aarch64/config.h"
#include "aarch64/mmu.h"
#include "libstd/vm.h"

[[clang::optnone]] [[gnu::naked]] [[noreturn]] void _() {
  asm volatile(
      R"(
.include "libstd/asm.hh"
.include "aarch64/asm_macros.h"

/*
 * Register use:
 *  x0-x3   Arguments
 *  x9-x15  Scratch
 *  x19-x28 Globals
 */
tmp                     .req x9
tmp2                    .req x10
wtmp2                   .req w10
idx                     .req x11
idx_shift               .req x12
page_table              .req x13
new_page_table          .req x14
phys_offset             .req x15

cpuid                   .req x19
page_table0             .req x20
page_table1             .req x21
mmu_initial_mapping     .req x22
vaddr                   .req x23
paddr                   .req x24
mapping_size            .req x25
size                    .req x26
attr                    .req x27
boot_el                 .req x28

.section .text.boot
FUNCTION _start
    /* keep track of the boot EL */
    mrs     boot_el, currentel

    /* if we came in at higher than EL1, drop down to EL1 */
    bl      arm64_elX_to_el1

    /* disable EL1 FPU traps */
    mov     tmp, #(0b11<<20)
    msr     cpacr_el1, tmp

    /* enable caches so atomics and spinlocks work */
    mrs     tmp, sctlr_el1
    orr     tmp, tmp, #(1<<12) /* Enable icache */
    orr     tmp, tmp, #(1<<2)  /* Enable dcache/ucache */
    orr     tmp, tmp, #(1<<3)  /* Enable Stack Alignment Check EL1 */
    orr     tmp, tmp, #(1<<4)  /* Enable Stack Alignment Check EL0 */
    bic     tmp, tmp, #(1<<1)  /* Disable Alignment Checking for EL1 EL0 */
    msr     sctlr_el1, tmp

    /* set up the mmu according to mmu_initial_mappings */

    /* load the base of the translation table and clear the table */
    adr_g   page_table1, arm64_kernel_translation_table

    /* Prepare tt_trampoline page table */
    /* Calculate pagetable physical addresses */
    adr_g   page_table0, tt_trampoline

    mrs     cpuid, mpidr_el1
    ubfx    cpuid, cpuid, #0, #%[SMP_CPU_ID_BITS]
    cbnz    cpuid, .Lmmu_enable_secondary

    /* this path forward until .Lmmu_enable_secondary is the primary cpu only */


    /* save a copy of the boot args so x0-x3 are available for use */
    adr_g   tmp, arm64_boot_args
    stp     x0, x1, [tmp], #16
    stp     x2, x3, [tmp]

    /* save the boot EL */
    adrp    tmp, arm64_boot_el
    str     boot_el, [tmp, #:lo12:arm64_boot_el]

    /* walk through all the entries in the translation table, setting them up */
    mov     tmp, #0
.Lclear_top_page_table_loop:
    str     xzr, [page_table1, tmp, lsl #3]
    add     tmp, tmp, #1
    cmp     tmp, #%[MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP]
    bne     .Lclear_top_page_table_loop

    /* load the address of the mmu_initial_mappings table and start processing */
    adr_g   mmu_initial_mapping, mmu_initial_mappings

.Linitial_mapping_loop:
/* Read entry of mmu_initial_mappings (likely defined in platform.c) */
    ldp     paddr, vaddr, [mmu_initial_mapping, #%[MMU_INITIAL_MAPPING_PHYS_OFFSET]]
    ldp     size, tmp, [mmu_initial_mapping, #%[MMU_INITIAL_MAPPING_SIZE_OFFSET]]

    tbzmask tmp, %[MMU_INITIAL_MAPPING_FLAG_DYNAMIC], .Lnot_dynamic
    adr     paddr, _start
    mov     size, x0 /* use the arg passed through from platform_reset */
    str     paddr, [mmu_initial_mapping, #%[MMU_INITIAL_MAPPING_PHYS_OFFSET]]
    str     size, [mmu_initial_mapping, #%[MMU_INITIAL_MAPPING_SIZE_OFFSET]]

.Lnot_dynamic:
    /* if size == 0, end of list, done with initial mapping */
    cbz     size, .Linitial_mapping_done
    mov     mapping_size, size

    /* set up the flags */
    tbzmask tmp, %[MMU_INITIAL_MAPPING_FLAG_UNCACHED], .Lnot_uncached
    ldr     attr, =%[MMU_INITIAL_MAP_STRONGLY_ORDERED]
    b       .Lmem_type_done

.Lnot_uncached:
    /* is this memory mapped to device/peripherals? */
    tbzmask tmp, %[MMU_INITIAL_MAPPING_FLAG_DEVICE], .Lnot_device
    ldr     attr, =%[MMU_INITIAL_MAP_DEVICE]
    b       .Lmem_type_done
.Lnot_device:

/* 如果 mmu_initial_mapping 的 flag 不是 DYNAMIC,UUCACHED,DEVICE，则通过链接脚本中的地址和vaddr做比较来决定链接脚本中的地址的内存属性*/
/* if mmu_initial_mapping flag is not DYNAMIC,UUCACHED,DEVICE, then decide the memory attribute */
/* Determine the segment in which the memory resides and set appropriate
 *  attributes.  In order to handle offset kernels, the following rules are
 *  implemented below:
 *      KERNEL_BASE    to __code_start             -read/write (see note below)
 *      __code_start   to __rodata_start (.text)   -read only
 *      __rodata_start to __data_start   (.rodata) -read only, execute never
 *      __data_start   to .....          (.data)   -read/write
 *
 *  The space below __code_start is presently left as read/write (same as .data)
 *   mainly as a workaround for the raspberry pi boot process.  Boot vectors for
 *   secondary CPUs are in this area and need to be updated by cpu0 once the system
 *   is ready to boot the secondary processors.
 *   TODO: handle this via mmu_initial_mapping entries, which may need to be
 *         extended with additional flag types
 */
.Lmapping_size_loop:
    ldr     attr, =%[MMU_PTE_KERNEL_DATA_FLAGS]
    ldr     tmp, =__code_start // __code_start==0xffff000000080000
    subs    size, tmp, vaddr
    /* If page is below the entry point (_start) mark as kernel data */
    b.hi    .Lmem_type_done

    ldr     attr, =%[MMU_PTE_KERNEL_RO_FLAGS]
    ldr     tmp, =__rodata_start
    subs    size, tmp, vaddr
    b.hi    .Lmem_type_done
    orr     attr, attr, #%[MMU_PTE_ATTR_PXN]
    ldr     tmp, =__data_start
    subs    size, tmp, vaddr
    b.hi    .Lmem_type_done
    ldr     attr, =%[MMU_PTE_KERNEL_DATA_FLAGS]
    ldr     tmp, =_end
    subs    size, tmp, vaddr
    b.lo    . /* Error: _end < vaddr */
    cmp     mapping_size, size
    b.lo    . /* Error: mapping_size < size => RAM size too small for data/bss */
    mov     size, mapping_size

.Lmem_type_done:
    subs    mapping_size, mapping_size, size
    b.lo    . /* Error: mapping_size < size (RAM size too small for code/rodata?) */

    /* Check that paddr, vaddr and size are page aligned */
    orr     tmp, vaddr, paddr
    orr     tmp, tmp, size
    tst     tmp, #(1 << %[MMU_KERNEL_PAGE_SIZE_SHIFT]) - 1
    bne     . /* Error: not page aligned */

    /* Clear top bits of virtual address (should be all set) */
    eor     vaddr, vaddr, #(~0 << %[MMU_KERNEL_SIZE_SHIFT])

    /* Check that top bits were all set */
    tst     vaddr, #(~0 << %[MMU_KERNEL_SIZE_SHIFT])
    bne     . /* Error: vaddr out of range */

.Lmap_range_top_loop:
    /* Select top level page table */
    mov     page_table, page_table1
    mov     idx_shift, #%[MMU_KERNEL_TOP_SHIFT]

    lsr     idx, vaddr, idx_shift


/* determine the type of page table entry to use given alignment and size
 *  of the chunk of memory we are mapping
 */
.Lmap_range_one_table_loop:
    /* Check if current level allow block descriptors */
    cmp     idx_shift, #%[MMU_PTE_DESCRIPTOR_BLOCK_MAX_SHIFT]
    b.hi    .Lmap_range_need_page_table

    /* Check if paddr and vaddr alignment allows a block descriptor */
    orr     tmp2, vaddr, paddr
    lsr     tmp, tmp2, idx_shift
    lsl     tmp, tmp, idx_shift
    cmp     tmp, tmp2
    b.ne    .Lmap_range_need_page_table

    /* Check if size is large enough for a block mapping */
    lsr     tmp, size, idx_shift
    cbz     tmp, .Lmap_range_need_page_table

    /* Select descriptor type, page for level 3, block for level 0-2 */
    orr     tmp, attr, #%[MMU_PTE_L3_DESCRIPTOR_PAGE]
    cmp     idx_shift, %[MMU_KERNEL_PAGE_SIZE_SHIFT]
    beq     .Lmap_range_l3
    orr     tmp, attr, #%[MMU_PTE_L012_DESCRIPTOR_BLOCK]
.Lmap_range_l3:

    /* Write page table entry */
    orr     tmp, tmp, paddr
    str     tmp, [page_table, idx, lsl #3]

    /* Move to next page table entry */
    mov     tmp, #1
    lsl     tmp, tmp, idx_shift
    add     vaddr, vaddr, tmp
    add     paddr, paddr, tmp
    subs    size, size, tmp
    /* TODO: add local loop if next entry is in the same page table */
    b.ne    .Lmap_range_top_loop /* size != 0 */

    /* Restore top bits of virtual address (should be all set) */
    eor     vaddr, vaddr, #(~0 << %[MMU_KERNEL_SIZE_SHIFT])
    /* Move to next subtype of ram mmu_initial_mappings entry */
    cbnz     mapping_size, .Lmapping_size_loop

    /* Move to next mmu_initial_mappings entry */
    add     mmu_initial_mapping, mmu_initial_mapping, %[MMU_INITIAL_MAPPING_SIZE]
    b       .Linitial_mapping_loop

.Lmap_range_need_page_table:
    /* Check if page table entry is unused */
    ldr     new_page_table, [page_table, idx, lsl #3]
    cbnz    new_page_table, .Lmap_range_has_page_table

    /* Calculate phys offset (needed for memory allocation) */
.Lphys_offset:
    adr     phys_offset, .Lphys_offset /* phys */
    ldr     tmp, =.Lphys_offset /* virt */
    sub     phys_offset, tmp, phys_offset

    /* Allocate new page table */
    calloc_bootmem_aligned new_page_table, tmp, tmp2, %[MMU_KERNEL_PAGE_SIZE_SHIFT], phys_offset

    /* Write page table entry (with allocated page table) */
    orr     new_page_table, new_page_table, #%[MMU_PTE_L012_DESCRIPTOR_TABLE]
    str     new_page_table, [page_table, idx, lsl #3]

.Lmap_range_has_page_table:
    /* Check descriptor type */
    and     tmp, new_page_table, #%[MMU_PTE_DESCRIPTOR_MASK]
    cmp     tmp, #%[MMU_PTE_L012_DESCRIPTOR_TABLE]
    b.ne    . /* Error: entry already in use (as a block entry) */

    /* switch to next page table level */
    bic     page_table, new_page_table, #%[MMU_PTE_DESCRIPTOR_MASK]
    mov     tmp, #~0
    lsl     tmp, tmp, idx_shift
    bic     tmp, vaddr, tmp
    sub     idx_shift, idx_shift, #(%[MMU_KERNEL_PAGE_SIZE_SHIFT] - 3)
    lsr     idx, tmp, idx_shift

    b       .Lmap_range_one_table_loop

.Linitial_mapping_done:

    /* Prepare tt_trampoline page table */

    /* Zero tt_trampoline translation tables */
    mov     tmp, #0
.Lclear_tt_trampoline:
    str     xzr, [page_table0, tmp, lsl#3]
    add     tmp, tmp, #1
    cmp     tmp, #%[MMU_PAGE_TABLE_ENTRIES_IDENT]
    blt     .Lclear_tt_trampoline

    /* Setup mapping at phys -> phys */
    adr     tmp, .Lmmu_on_pc
    lsr     tmp, tmp, #%[MMU_IDENT_TOP_SHIFT]    /* tmp = paddr idx */
    ldr     tmp2, =%[MMU_PTE_IDENT_FLAGS]
    add     tmp2, tmp2, tmp, lsl #%[MMU_IDENT_TOP_SHIFT]  /* tmp2 = pt entry */

    str     tmp2, [page_table0, tmp, lsl #3]     /* tt_trampoline[paddr idx] = pt entry */

    adrp    tmp, page_tables_not_ready
    add     tmp, tmp, #:lo12:page_tables_not_ready
    str     wzr, [tmp]
    b       .Lpage_tables_ready

.Lmmu_enable_secondary:
    adrp    tmp, page_tables_not_ready
    add     tmp, tmp, #:lo12:page_tables_not_ready
.Lpage_tables_not_ready:
    ldr     wtmp2, [tmp]
    cbnz    wtmp2, .Lpage_tables_not_ready
.Lpage_tables_ready:

    /* set up the mmu */

    /* Invalidate TLB */
    tlbi    vmalle1is
    dsb     sy
    isb

    /* Initialize Memory Attribute Indirection Register */
    ldr     tmp, =%[MMU_MAIR_VAL]
    msr     mair_el1, tmp

    /* Initialize TCR_EL1 */
    /* set cacheable attributes on translation walk */
    /* (SMP extensions) non-shareable, inner write-back write-allocate */
    ldr     tmp, =%[MMU_TCR_FLAGS_IDENT]
    msr     tcr_el1, tmp

    isb

    /* Write ttbr with phys addr of the translation table */
    msr     ttbr0_el1, page_table0
    msr     ttbr1_el1, page_table1
    isb

    /* Read SCTLR */
    mrs     tmp, sctlr_el1

    /* Turn on the MMU */
    orr     tmp, tmp, #0x1

    /* Write back SCTLR */
    msr     sctlr_el1, tmp
.Lmmu_on_pc:
    isb

    /* Jump to virtual code address */
    ldr     tmp, =.Lmmu_on_vaddr
    br      tmp

.Lmmu_on_vaddr:

    /* Disable trampoline page-table in ttbr0 */
    ldr     tmp, =%[MMU_TCR_FLAGS_KERNEL]
    msr     tcr_el1, tmp
    isb


    /* Invalidate TLB */
    tlbi    vmalle1
    dsb     sy
    isb

    cbnz    cpuid, .Lsecondary_boot

    /* load the stack pointer */
    ldr tmp, =__stack_end
    mov sp, tmp

.if 0
// todo: following code not working
    /* clear bss */
.L__do_bss:
    /* clear out the bss excluding the stack and kernel translation table  */
    /* NOTE: relies on __post_prebss_bss_start and __bss_end being 8 byte aligned */
    ldr     tmp, =__post_prebss_bss_start
    ldr     tmp2, =__bss_end
    sub     tmp2, tmp2, tmp
    cbz     tmp2, .L__bss_loop_done
.L__bss_loop:
    sub     tmp2, tmp2, #8
    str     xzr, [tmp], #8
    cbnz    tmp2, .L__bss_loop
.L__bss_loop_done:
.endif

    /* load the boot args we had saved previously */
    adrp    tmp, arm64_boot_args
    add     tmp, tmp, :lo12:arm64_boot_args
    ldp     x0, x1, [tmp], #16
    ldp     x2, x3, [tmp]

    bl  kernel_main
    b   .

.Lsecondary_boot:
    and     tmp, cpuid, #0xff
    cmp     tmp, #(1 << %[SMP_CPU_CLUSTER_SHIFT])
    bge     .Lunsupported_cpu_trap
    bic     cpuid, cpuid, #0xff
    orr     cpuid, tmp, cpuid, LSR #(8 - %[SMP_CPU_CLUSTER_SHIFT])

    cmp     cpuid, #%[SMP_MAX_CPUS]
    bge     .Lunsupported_cpu_trap

    /* Set up the stack */
    ldr     tmp, =__stack_end
    mov     tmp2, #%[ARCH_DEFAULT_STACK_SIZE]
    mul     tmp2, tmp2, cpuid
    sub     sp, tmp, tmp2

    mov     x0, cpuid
    bl      arm64_secondary_entry

.Lunsupported_cpu_trap:
    wfe
    b       .Lunsupported_cpu_trap

.ltorg

.data
    .balign 8
LOCAL_DATA arm64_boot_args
    .skip (4 * 8)
END_DATA arm64_boot_args

DATA arm64_boot_el
    .skip 8
END_DATA arm64_boot_el

.data
DATA page_tables_not_ready
    .long       1
END_DATA page_tables_not_ready

.section .bss.prebss.stack
    .align 4
DATA __stack
    .skip %[ARCH_DEFAULT_STACK_SIZE] * %[SMP_MAX_CPUS]
END_DATA __stack

DATA __stack_end

.section ".bss.prebss.translation_table"
.align 3 + %[MMU_PAGE_TABLE_ENTRIES_IDENT_SHIFT]
DATA tt_trampoline
    .skip 8 * %[MMU_PAGE_TABLE_ENTRIES_IDENT]
END_DATA tt_trampoline
)"
      :
      // clang-format off
      : [SMP_CPU_ID_BITS] "i"(SMP_CPU_ID_BITS),
        [SMP_CPU_CLUSTER_SHIFT] "i"(SMP_CPU_CLUSTER_SHIFT),
        [SMP_MAX_CPUS] "i"(SMP_MAX_CPUS),

        [ARCH_DEFAULT_STACK_SIZE] "i"(ARCH_DEFAULT_STACK_SIZE),

        [MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP] "i"(MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP),
        [MMU_KERNEL_PAGE_SIZE_SHIFT] "i"(MMU_KERNEL_PAGE_SIZE_SHIFT),
        [MMU_KERNEL_SIZE_SHIFT] "i"(MMU_KERNEL_SIZE_SHIFT()),
        [MMU_KERNEL_TOP_SHIFT] "i"(MMU_KERNEL_TOP_SHIFT()),

        [MMU_INITIAL_MAPPING_PHYS_OFFSET] "i"(MMU_INITIAL_MAPPING_PHYS_OFFSET()),
        [MMU_INITIAL_MAPPING_SIZE_OFFSET] "i"(MMU_INITIAL_MAPPING_SIZE_OFFSET()),
        [MMU_INITIAL_MAPPING_FLAG_DYNAMIC] "i"(MMU_INITIAL_MAPPING_FLAG_DYNAMIC),
        [MMU_INITIAL_MAPPING_FLAG_UNCACHED] "i"(MMU_INITIAL_MAPPING_FLAG_UNCACHED),
        [MMU_INITIAL_MAP_STRONGLY_ORDERED] "i"(MMU_INITIAL_MAP_STRONGLY_ORDERED),
        [MMU_INITIAL_MAPPING_FLAG_DEVICE] "i"(MMU_INITIAL_MAPPING_FLAG_DEVICE),
        [MMU_INITIAL_MAPPING_SIZE] "i"(MMU_INITIAL_MAPPING_SIZE()),
        [MMU_INITIAL_MAP_DEVICE] "i"(MMU_INITIAL_MAP_DEVICE),

        [MMU_PTE_IDENT_FLAGS] "i"(MMU_PTE_IDENT_FLAGS),
        [MMU_PTE_KERNEL_DATA_FLAGS] "i"(MMU_PTE_KERNEL_DATA_FLAGS),
        [MMU_PTE_KERNEL_RO_FLAGS] "i"(MMU_PTE_KERNEL_RO_FLAGS),
        [MMU_PTE_ATTR_PXN] "i"(MMU_PTE_ATTR_PXN),
        [MMU_PTE_DESCRIPTOR_BLOCK_MAX_SHIFT] "i"(MMU_PTE_DESCRIPTOR_BLOCK_MAX_SHIFT),
        [MMU_PTE_L3_DESCRIPTOR_PAGE] "i"(MMU_PTE_L3_DESCRIPTOR_PAGE),
        [MMU_PTE_L012_DESCRIPTOR_BLOCK] "i"(MMU_PTE_L012_DESCRIPTOR_BLOCK),
        [MMU_PTE_L012_DESCRIPTOR_TABLE] "i"(MMU_PTE_L012_DESCRIPTOR_TABLE),
        [MMU_PTE_DESCRIPTOR_MASK] "i"(MMU_PTE_DESCRIPTOR_MASK),

        [MMU_PAGE_TABLE_ENTRIES_IDENT] "i"(MMU_PAGE_TABLE_ENTRIES_IDENT),
        [MMU_PAGE_TABLE_ENTRIES_IDENT_SHIFT] "i"(MMU_PAGE_TABLE_ENTRIES_IDENT_SHIFT),

        [MMU_TCR_FLAGS_IDENT] "i"(MMU_TCR_FLAGS_IDENT),
        [MMU_TCR_FLAGS_KERNEL] "i"(MMU_TCR_FLAGS_KERNEL),
        [MMU_IDENT_TOP_SHIFT] "i"(MMU_IDENT_TOP_SHIFT()),
        [MMU_MAIR_VAL] "i"(MMU_MAIR_VAL)
      // clang-format on
      : "cc", "memory");
}
