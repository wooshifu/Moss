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

    __rodata_start = .;
    .rodata :
    {
        *(.rodata)
    }
    . = ALIGN(8);
    __rodata_end = .;

    __data_start = .;
    .data :
    {
        *(.data)
    }
    . = ALIGN(4096);
    l0_page_table = .;
    . += 4096;
    __data_end = .;

    __bss_start = .;
    .bss :
    {
        bss = .;
        *(.bss)
    }
    . = ALIGN(8);
    __bss_end = .;
}
