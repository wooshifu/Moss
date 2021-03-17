.section ".text.boot"

.global _start
_start:
    // read cpu id, stop slave cores
    mrs     x1, mpidr_el1            // MPIDR_EL1: Multi-Processor Affinity Register
    and     x1, x1, #0xFFFF
    cbz     x1, .L__cpu_0             // .L prefix is the local label in ELF

// cpu id > 0, stop
// cpu id == 0 will also goto here after returned from kernel_main() if possible
.L__current_cpu_idle:
wfi
    b       .L__current_cpu_idle

.L__cpu_0:  // cpu id == 0
    mrs     x0, CurrentEL           // CurrentEL Register. bit 2, 3. Others reserved
    lsr     x0, x0, #2

    // running at EL3?
    cmp     x0, #3                // EL3
    bne     .L__not_in_el3

    // should never be executed, just for completeness. (EL3)
    mov     x2, #0b10110110001
    msr     scr_el3, x2            // SCR_ELn  Secure Configuration Register
    mov     x2, #0b1111001001      // [0:3]0b1001->EL2h, [6:9]0b1111->DAIF interrupt mask
    msr     spsr_el3, x2           // SPSR_ELn. Saved Program Status Register. 1111001001
    adr     x2, .L__not_in_el3
    msr     elr_el3, x2
    eret                           // Exception Return: from EL3, continue from .L__not_in_el3

// running at EL2 or EL1
.L__not_in_el3:
    // should we check el0? it's not possible booting at el0?
    // cmp     x0, #0              // EL0
    // beq     .L__in_el0

    cmp     x0, #1                 // EL1
    beq     .L__in_el1

    // in EL2
    ldr     x1, =_start
    msr     sp_el1, x1             // Set sp of EL1 to _start

    // enable CNTP for EL2
    mrs     x0, cnthctl_el2         // Counter-timer Hypervisor Control register
    orr     x0, x0, #3
    msr     cnthctl_el2, x0
    msr     cntvoff_el2, xzr

    // enable AArch64 in EL2
    mov     x0, #(1 << 31)          // AArch64
    orr     x0, x0, #(1 << 1)       // SWIO hardwired on Pi3
    msr     hcr_el2, x0
    mrs     x0, hcr_el2

    // change execution level to EL1
    mov     x2, #0b1111000100       // [0:3]0b0100->EL1t, [6:9]0b1111->DAIF interrupt mask
    msr     spsr_el2, x2
    adr     x2, .L__in_el1
    msr     elr_el2, x2
    eret                            // exception return. from EL2. continue from .L__in_el1

.L__in_el1:
    // sp 必须在 mmu 映射的范围内
    // sp must be in mmu mapped address
    mov     sp, x1             // in EL1. Set sp to _start

    // Set CPACR_EL1 (Architecture Feature Access Control Register) to avoid trap from SIMD or float point instruction
    mov     x1, #0x00300000       // Don't trap any SIMD/FP instructions in both EL0 and EL1
    msr     cpacr_el1, x1

    mrs     x1, sctlr_el1
    orr     x1, x1, #(1 << 12)
    bic     x1, x1, #(3 << 3)
    bic     x1, x1, #(1 << 1)
    msr     sctlr_el1, x1

    // clear bss
    bl      bss_clean

// jump to C code, should not return
.L__jump_to_main:
#ifndef ENV_KERNEL_UNIT_TESTS
    bl      kernel_main
#else
    bl      kernel_main_tests
#endif
    // for failsafe, halt this core too
    b       .L__current_cpu_idle
