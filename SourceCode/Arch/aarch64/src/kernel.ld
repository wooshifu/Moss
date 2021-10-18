ENTRY(__text_start)

SECTIONS
{
    . = 0x80000;
    .text :
    {
        __kernel_start = .;
        __text_start = .;
            *(.text.boot)
            *(.text)
        __text_end = .;
    }

    .rodata :
    {
        . = ALIGN(8);
        __rodata_start = .;
            *(.rodata)
            *(.rodata.*)
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
        /* align 64 bytes to fit assembly code .Lbss_loop */
        . = ALIGN(64);
        __bss_start = .;
            *(.bss)
        . = ALIGN(64);
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

    .note.gnu.build-id : { *(.note.gnu.build-id) }
    .interp : { *(.interp) }

    /* strip unnecessary stuff */
    /DISCARD/ : { *(.comment .note .eh_frame) }

    __kernel_end = .;
}
