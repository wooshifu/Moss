ENTRY(__text_start)

SECTIONS
{
    /*. = 0xffff000000080000;*/
    . = 0x80000;
    .text : /*AT(0x00000000 + 0x00080000)*/
    {
        __text_start = .;
            *(.text.boot)
            *(.text)

        . = ALIGN(8);
        __init_hooks_start = .;
            *(.init_hook)
        __init_hooks_end = .;

        __text_end = .;
    }

    .rodata :
    {
        . = ALIGN(8);
        __rodata_start = .;
            *(.rodata)
        __rodata_end = .;
    }

    .data :
    {
        . = ALIGN(8);
        __data_start = .;
            *(.data)
        __data_end = .;
    }

    /* must be aligned */
    .bss :
    {
        . = ALIGN(8);
        __bss_start = .;
            *(.bss)
        __bss_end = .;
    }

    .stack :
    {
        . = ALIGN(8);
        __stack_start = .;
        . += 40960;
        . = ALIGN(8);
        __stack_end = .;
    }

    .interp : { *(.interp) }

    /* strip unnecessary stuff */
    /DISCARD/ : { *(.comment .note .eh_frame) }
}
