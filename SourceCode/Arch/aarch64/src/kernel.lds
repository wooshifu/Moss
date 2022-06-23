OUTPUT_FORMAT("elf64-littleaarch64", "elf64-bigaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)

ENTRY(_start)
SECTIONS
{
    /* kernel_relocated_base=0xffffffff00000000 */
    /* the load address is kernel.elf entrypoint */
    /* because all code is (position independent code)pic, os the address is mainly for gdb debugging */
    . = 0xffffffff00000000;

    /* text/read-only data */
    /* set the load address to physical MEMBASE */
    .text : AT(0x80000) {
        __code_start = .;
        KEEP(*(.text.boot))
        *(.text* .gnu.linkonce.t.*)
    }

    INCLUDE "lds/common.lds"

    .dummy_post_text : {
	    __code_end = .;
    }

    INCLUDE "lds/rodata.lds"

    INCLUDE "lds/data.lds"

    INCLUDE "lds/bss.lds"

    /* Align the end to ensure anything after the kernel ends up on its own pages */
    . = ALIGN(4096);
    _end = .;

    /* Strip unnecessary stuff */
    /DISCARD/ : { *(.comment .note .eh_frame) }
}
