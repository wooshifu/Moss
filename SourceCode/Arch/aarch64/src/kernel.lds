OUTPUT_FORMAT("elf64-littleaarch64", "elf64-bigaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)

ENTRY(_start)
SECTIONS
{
    /* kernel_relocated_base=0xffffffff00000000 */
    /* the load address is kernel.elf entrypoint */
    /* all code is (position independent code)pic, so the address is mainly for gdb debugging */
    . = 0xffffffff00000000;

    /* text/read-only data */
    /* set the load address to physical MEMBASE */
    .text : AT(0x80000) {
        __code_start = .;
        KEEP(*(.text.boot))
        *(.text*)
    }

    INCLUDE "lds/common.lds"

    .dummy_post_text : {
	    __code_end = .;
    }

    INCLUDE "lds/data.lds"

    /* Align the end to ensure anything after the kernel ends up on its own pages */
    . = ALIGN(4096);
    _end = .;

    /* Strip unnecessary stuff */
    /DISCARD/ : { *(.comment .note .eh_frame) }
}
