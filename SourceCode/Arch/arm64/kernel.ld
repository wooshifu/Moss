ENTRY(__text_start)

SECTIONS
{
    . = 0x8000;
    __text_start = .;
    .text :
    {
        KEEP(*(.interrupt_vector_table))
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
    . = ALIGN(8);
    __data_end = .;

    __bss_start = .;
    .bss :
    {
        bss = .;
        *(.bss)
    }
    . = ALIGN(8);
    __bss_end = .;

    . = 0x20000 + __bss_end;
    __svc_stack = .;

    . = 0x20000 + __svc_stack;
    __irq_stack = .;

    . = 0x20000 + __irq_stack;
    __fiq_stack = .;

    . = 0x20000 + __fiq_stack;
    __sys_stack = .;

    __end_stack = .;
    . = ALIGN(8);

    __PAGE_TABLE = .;
    . = 0x805000 + __PAGE_TABLE;

    __HEAP_BEGIN = .;
}
