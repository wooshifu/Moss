#include "sysregs.h"

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

.macro _print_current_el
    bl   get_current_el
    mov  x1, x0
    ldr  x0, =__message
    bl   printf_
.endm

_init_core0:
#if 0
    ldr  x0, =0x80000
    mov  sp, x0

    _print_current_el

    // important!!!
    mov x0, #3 << 20
    msr cpacr_el1, x0           /* Enable FP/SIMD at EL1 */

	ldr	x0, =SCTLR_VALUE_MMU_DISABLED
	msr	sctlr_el1, x0

	ldr	x0, =HCR_VALUE
	msr	hcr_el2, x0

	ldr	x0, =SCR_VALUE
	msr	scr_el3, x0

	ldr	x0, =SPSR_VALUE
	msr	spsr_el3, x0

	adr	x0, _goto_kernel_main
	msr	elr_el3, x0

	eret
#endif

#if 1
drop_el1_secure:
/* Try drop from el3 to el1 secure */

/*=============================================================*/
/*      Enable FP/SIMD at EL1                                  */
/*=============================================================*/
mov x0, #(0b11 << 20)
msr cpacr_el1, x0           /* Enable FP/SIMD at EL1 */

/*=============================================================*/
/*      Initialize sctlr_el1                                   */
/*=============================================================*/
mov x0, xzr
orr x0, x0, #(1 << 29)          /* Checking http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0500d/CIHDIEBD.html */
orr x0, x0, #(1 << 28)          /* Bits 29,28,23,22,20,11 should be 1 (res1 on documentation) */
orr x0, x0, #(1 << 23)
orr x0, x0, #(1 << 22)
orr x0, x0, #(1 << 20)
orr x0, x0, #(1 << 11)
msr sctlr_el1, x0

/*=============================================================*/
/*      Initialize scr_el3                                     */
/*=============================================================*/
mrs x0, scr_el3
orr x0, x0, #(1<<10)        /* Lower EL is 64bits */
msr scr_el3, x0

/*=============================================================*/
/*      Initialize spsr_el3                                    */
/*=============================================================*/
mov x0, xzr
mov x0, #0b00101            /* EL1 */
orr x0, x0, #(1 << 8)       /* Enable SError and External Abort. */
orr x0, x0, #(1 << 7)       /* IRQ interrupt Process state mask. */
orr x0, x0, #(1 << 6)       /* FIQ interrupt Process state mask. */
msr spsr_el3, x0

/*=============================================================*/
/*      Initialize elr_el3                                     */
/*=============================================================*/
adr x0, _goto_kernel_main
msr elr_el3, x0

eret
#endif



    //b    _goto_kernel_main
    //b    _hang_core // should never come here

    // TODO: before we can use mmu, we should enter EL1, because TTBR1_EL1 is not available in EL2 and EL3

_goto_kernel_main:
    //ldr  x0, =__sys_stack
    ldr  x0, =0x80000
    mov  sp, x0

    bl   print_mem_stack
    bl   bss_clean

    bl   get_current_el
    mov  x1,x0
    ldr  x0, =__message
    bl   printf_
#ifndef ENV_KERNEL_UNIT_TESTS
    bl   kernel_main
#else
    bl   kernel_main_tests
#endif
    mrs  x0, mpidr_el1
    and  x0, x0, #0xFFFF // check processor id
    mov  x1, x0
    ldr  x0, =__core_hang_message
    bl   printf_
    b    _hang_core // kernel_main should never come here

__message:
    .asciz "[DEBUG] current el: %d\n"

__core_hang_message:
    .asciz "[DEBUG] core %d hanging \n"
