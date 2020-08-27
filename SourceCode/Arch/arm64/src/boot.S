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

    bl get_current_el
    mov x1,x0
    ldr x0, =__message
    bl printf_

    b    _goto_kernel_main
    b    _hang_core // should never come here

    // TODO: before we can use mmu, we should enter EL1, because TTBR1_EL1 is not available in EL2 and EL3

_goto_kernel_main:
#ifndef ENV_KERNEL_UNIT_TESTS
    bl   kernel_main
#else
    bl   kernel_main_tests
#endif

__message:
    .asciz "[DEBUG] current el: %d\n"