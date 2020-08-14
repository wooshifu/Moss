.section ".interrupt_vector_table"
exception_vector_table:
    b _reset_handler
    b _undefined_instruction_handler
    b _software_interrupt_handler
    b _prefetch_abort_handler
    b _data_abort_handler
    b _unused_handler
    b _interrupt_handler
    b _fast_interrupt_handler

_reset_handler:
    mrc p15, #0, r0, c0, c0, #5 // Move to ARM Register r0 from Coprocessor c0. Read ID Code Register
    and r0, r0, #3 // r0 &= 0x3
    cmp r0, #0 // check whether r0==0
    beq _reset_cpu0 // reset cpu0

    ldr pc,=halt_cpu

_reset_cpu0:
    ; push    {r4, r5, r6, r7, r8, r9}

    ldr     r0, =exception_vector_table

    // set vector base address
    mcr p15, 0, r0, c12, c0, 0

    ;mov     r1, #0x0000
    ;ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    ;stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    ;ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8}
    ;stmia   r1!,{r2, r3, r4, r5, r6, r7, r8}
    ;pop     {r4, r5, r6, r7, r8, r9}
    ldr     pc, =_start


halt_cpu:
    wfi // wait for interrup coming
    b halt_cpu

// todo not implemented
_undefined_instruction_handler:
    nop
_software_interrupt_handler:
    nop
_prefetch_abort_handler:
    nop
_data_abort_handler:
    nop
_unused_handler:
    nop
_interrupt_handler:
    nop
_fast_interrupt_handler:
    nop




.section ".text.boot"
.globl _start
_start:
    mov	r0, #0
    mcr	p15, 0, r0, c12, c0, 0

    // there are 4 cores within rasberry pi 3, and 4 cores will execute the following code at the same time
    mrc p15, #0, r0, c0, c0, #5 // Move to ARM Register r0 from Coprocessor c0. Read ID Code Register
    and r0, r0, #0xFF // r0 &= 0xFF
    cmp r0, #0 // check whether r0==0
    beq _init_cpu0 // init cpu0

    ldr pc, =_halt_smp // else, go to _halt_smp, it means only core with id 0,1,2,3 will execute the following code

_init_cpu0:
    ldr sp, =__sys_stack

    ldr r0, =__bss_start
    ldr r1, =__bss_end
    bl  memclean // call memclean(r0, r1)

    mrs r0, cpsr // move coprocessor register cpsr to ARM register r0

    bic r0,     #0x1F // set r0[4:0] to bit 0
    orr r0,     #0x13 // set r0[4], r0[1:0] to bit 1, 0b10011
    msr cpsr_c, r0 // enter SVC (supervisor) mode
    ldr r1,=__svc_stack
    bic sp, r1, #0x7 // set r1[2:0] to 0, and assign it to sp. 4k align

    bic r0,     #0x1F
    orr r0,     #0x12 // set r0[4], r0[1] to bit 1, 0b10010
    msr cpsr_c, r0 // enter IRQ mode
    ldr r1,=__irq_stack
    bic sp, r1, #0x7

    bic r0,     #0x1F
    orr r0,     #0x11 // set r0[4], r0[0] to bit 1, 0b10001
    msr cpsr_c, r0 // enter FIQ mode
    ldr r1,=__fiq_stack
    bic sp, r1, #0x7

    bic r0,     #0x1F
    orr r0,     #0x13
    msr cpsr_c, r0 // enter SVC (supervisor) mode
    ldr pc, =_goto_kernel_main

_goto_kernel_main:
    #ifndef ENV_KERNEL_UNIT_TESTS
        bl kernel_main
    #else
;    #warning "building kernel for unit tests!!!"
        bl kernel_main_tests
    #endif

_halt_smp:
    wfi // wait for interrup coming
    b _halt_smp
