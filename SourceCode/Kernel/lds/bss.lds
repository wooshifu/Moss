
    /* unintialized data (in same segment as writable data) */
    .bss : ALIGN(4096) {
        __bss_start = .;
        KEEP(*(.bss.prebss.*))
        . = ALIGN(8);
        __post_prebss_bss_start = .;
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end = .;
    }
