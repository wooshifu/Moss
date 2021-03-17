ENTRY(__text_start)

SECTIONS
{
    . = 0x80000;
    __text_start = .;
    .text :
    {
        KEEP(*(.text.boot))
        *(.text)
    }

    . = ALIGN(8);
    __pre_init_board_hook_start = .;
    .pre_init_board_hook :
    {
        *(.pre_init_board_hook)
    }
    . = ALIGN(8);
    __pre_init_board_hook_end = .;

    . = ALIGN(8);
    __post_init_board_hook_start = .;
    .post_init_board_hook :
    {
        *(.post_init_board_hook)
    }
    . = ALIGN(8);
    __post_init_board_hook_end = .;

    . = ALIGN(8);
    __pre_init_cpu_hook_start = .;
    .pre_init_cpu_hook :
    {
        *(.pre_init_cpu_hook)
    }
    . = ALIGN(8);
    __pre_init_cpu_hook_end = .;

    . = ALIGN(8);
    __post_init_cpu_hook_start = .;
    .post_init_cpu_hook :
    {
        *(.post_init_cpu_hook)
    }
    . = ALIGN(8);
    __post_init_cpu_hook_end = .;

    . = ALIGN(8);
    __pre_init_mmu_hook_start = .;
    .pre_init_mmu_hook :
    {
        *(.pre_init_mmu_hook)
    }
    . = ALIGN(8);
    __pre_init_mmu_hook_end = .;

    . = ALIGN(8);
    __post_init_mmu_hook_start = .;
    .post_init_mmu_hook :
    {
        *(.post_init_mmu_hook)
    }
    . = ALIGN(8);
    __post_init_mmu_hook_end = .;

    . = ALIGN(8);
    __pre_kernel_main_hook_start = .;
    .pre_kernel_main_hook :
    {
        *(.pre_kernel_main_hook)
    }
    . = ALIGN(8);
    __pre_kernel_main_hook_end = .;

    . = ALIGN(8);
    __post_kernel_main_hook_start = .;
    .post_kernel_main_hook :
    {
        *(.post_kernel_main_hook)
    }
    . = ALIGN(8);
    __post_kernel_main_hook_end = .;

    . = ALIGN(8);
    __text_end = .;

    . = ALIGN(8);
    __rodata_start = .;
    .rodata :
    {
        *(.rodata)
    }
    . = ALIGN(8);
    __rodata_end = .;

    . = ALIGN(8);
    __data_start = .;
    .data :
    {
        *(.data)
    }
    . = ALIGN(8);
    __data_end = .;

    . = ALIGN(8);
    __init_entry_start = .;
    .init_array :
    {
        *(.init_array)
    }
    . = ALIGN(8);
    __init_entry_end = .;

    /* must be aligned */
    . = ALIGN(8);
    __bss_start = .;
    .bss :
    {
        bss = .;
        *(.bss)
    }
    . = ALIGN(8);
    __bss_end = .;

    . = ALIGN(8);
    __symtab_start = .;
    .symtab :
    {
        *(.symtab)
    }
    . = ALIGN(8);
    __symtab_end = .;

    . = ALIGN(8);
    __strtab_start = .;
    .strtab :
    {
        *(.strtab)
    }
    . = ALIGN(8);
    __strtab_end = .;

    . = ALIGN(8);
    __interp_start = .;
    .interp :
    {
        *(.interp)
    }
    . = ALIGN(8);
    __interp_end = .;

    . = ALIGN(8);
    __build_id_start = .;
    .note.gnu.build-id :
    {
        *(.note.gnu.build-id)
    }
    . = ALIGN(8);
    __build_id_end = .;

    . = ALIGN(8);
    __stack_start = .;
    . += 40960;
    . = ALIGN(8);
    __stack_end = .;
}
