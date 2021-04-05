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
    __init_hooks_start = .;
    .init_hooks :
    {
        *(.init_hook)
    }
    __init_hooks_end = .;

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

/*
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
*/
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

    /* Strip unnecessary stuff */
    /DISCARD/ : { *(.comment .note .eh_frame) }
}
