.section ".text.boot"

.globl _start
_start:
    mrs  x0, mpidr_el1
    and  x0, x0, #0xFFFF // check processor id
    cbz  x0, _init_core0 // hang for all non-primary CPU
    b    _hang_core

_hang_core:
    wfi
    b    _hang_core

_init_core0:
    ldr  x0, =__sys_stack
    mov  sp, x0

    bl   print_mem_stack
    bl   bss_clean

    b    _goto_kernel_main
    b    _hang_core // should never come here

_goto_kernel_main:
#ifndef ENV_KERNEL_UNIT_TESTS
    bl   kernel_main
#else
    bl   kernel_main_tests
#endif
