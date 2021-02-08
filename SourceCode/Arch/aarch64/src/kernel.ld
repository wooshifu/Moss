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

    .interp :
    {
        *(.interp)
    }

    .note.gnu.build-id :
    {
        *(.note.gnu.build-id)
    }

    . = ALIGN(8);
    __stack_begin = .;
    . += 40960;
    . = ALIGN(8);
    __stack_end = .;
}
