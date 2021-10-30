//
// Created by shifu on 2021/10/17.
//

#include "libcxx/macros.hh"
#include "libcxx/types.hh"

constexpr auto S_FRAME_SIZE         = 256; // size of all saved registers

constexpr auto SYNC_INVALID_EL1t    = 0;
constexpr auto IRQ_INVALID_EL1t     = 1;
constexpr auto FIQ_INVALID_EL1t     = 2;
constexpr auto ERROR_INVALID_EL1t   = 3;

constexpr auto SYNC_INVALID_EL1h    = 4;
constexpr auto IRQ_INVALID_EL1h     = 5;
constexpr auto FIQ_INVALID_EL1h     = 6;
constexpr auto ERROR_INVALID_EL1h   = 7;

constexpr auto SYNC_INVALID_EL0_64  = 8;
constexpr auto IRQ_INVALID_EL0_64   = 9;
constexpr auto FIQ_INVALID_EL0_64   = 10;
constexpr auto ERROR_INVALID_EL0_64 = 11;

constexpr auto SYNC_INVALID_EL0_32  = 12;
constexpr auto IRQ_INVALID_EL0_32   = 13;
constexpr auto FIQ_INVALID_EL0_32   = 14;
constexpr auto ERROR_INVALID_EL0_32 = 15;

constexpr const char* entry_error_messages[]{
    "SYNC_INVALID_EL1t",   "IRQ_INVALID_EL1t",   "FIQ_INVALID_EL1t",   "ERROR_INVALID_EL1T",
    "SYNC_INVALID_EL1h",   "IRQ_INVALID_EL1h",   "FIQ_INVALID_EL1h",   "ERROR_INVALID_EL1h",
    "SYNC_INVALID_EL0_64", "IRQ_INVALID_EL0_64", "FIQ_INVALID_EL0_64", "ERROR_INVALID_EL0_64",
    "SYNC_INVALID_EL0_32", "IRQ_INVALID_EL0_32", "FIQ_INVALID_EL0_32", "ERROR_INVALID_EL0_32"};
static_assert(sizeof(entry_error_messages) / sizeof(entry_error_messages[0]) == 16);

// todo: remove #pragma
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
[[gnu::used]] extern_C void show_invalid_entry_message(int type, u64 far_el1, u64 sp, u64 esr_el1, u64 elr_el1) {
  //  log_e("[%s] far_el1: 0x%lx, sp: 0x%lx, esr_el1: 0x%lx, elr_el1: 0x%lx, exception class: %s",
  //        entry_error_messages[type], far_el1, sp, esr_el1, elr_el1, esr_get_class_string(esr_el1));
}
#pragma clang diagnostic pop

// todo: not implemented
extern_C void oops() {}

// todo: not implemented
extern_C void handle_el1_irq(void) {}

[[gnu::used]] extern_C void init_exception_vector_table() {
  asm volatile(R"(
  b init_exception_vector_table_asm


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "aarch64/exception.hh"

.macro exception_vector_entry label
.align 7  // 2^7=0x80, all exception vectors should be located at offset 0x80 bytes one from another
    b    \label
.endm


.macro handle_invalid_entry type
    kernel_entry
    mov    x0, #\type
    // todo: should not read el_1 register values if we are in el0
    mrs    x1, far_el1
    mov    x2, sp
    mrs    x3, esr_el1
    mrs    x4, elr_el1
    bl     show_invalid_entry_message
    // TODO: kernel_exit should we do this ???
    //b      err_hang
    bl oops
.endm

// todo: save not only generic registers
// save x0-x30 to stack
.macro	kernel_entry
    sub	sp, sp, %[S_FRAME_SIZE]
    stp	x0, x1, [sp, #16 * 0]
    stp	x2, x3, [sp, #16 * 1]
    stp	x4, x5, [sp, #16 * 2]
    stp	x6, x7, [sp, #16 * 3]
    stp	x8, x9, [sp, #16 * 4]
    stp	x10, x11, [sp, #16 * 5]
    stp	x12, x13, [sp, #16 * 6]
    stp	x14, x15, [sp, #16 * 7]
    stp	x16, x17, [sp, #16 * 8]
    stp	x18, x19, [sp, #16 * 9]
    stp	x20, x21, [sp, #16 * 10]
    stp	x22, x23, [sp, #16 * 11]
    stp	x24, x25, [sp, #16 * 12]
    stp	x26, x27, [sp, #16 * 13]
    stp	x28, x29, [sp, #16 * 14]
    str	x30, [sp, #16 * 15]
.endm

// todo: restore not only generic registers
// restore x0-x30 from stack
.macro	kernel_exit
    ldp	x0, x1, [sp, #16 * 0]
    ldp	x2, x3, [sp, #16 * 1]
    ldp	x4, x5, [sp, #16 * 2]
    ldp	x6, x7, [sp, #16 * 3]
    ldp	x8, x9, [sp, #16 * 4]
    ldp	x10, x11, [sp, #16 * 5]
    ldp	x12, x13, [sp, #16 * 6]
    ldp	x14, x15, [sp, #16 * 7]
    ldp	x16, x17, [sp, #16 * 8]
    ldp	x18, x19, [sp, #16 * 9]
    ldp	x20, x21, [sp, #16 * 10]
    ldp	x22, x23, [sp, #16 * 11]
    ldp	x24, x25, [sp, #16 * 12]
    ldp	x26, x27, [sp, #16 * 13]
    ldp	x28, x29, [sp, #16 * 14]
    ldr	x30, [sp, #16 * 15]
    add	sp, sp, %[S_FRAME_SIZE]
    eret
.endm



/// exception vectors table.
/// about EL1t and EL1h, see https://developer.arm.com/documentation/ddi0488/c/programmers-model/armv8-architecture-concepts/stack-pointer-selection
.align 11 // 2k align, vbar_el1 [11:0] is RES0, so 2k align is required
.globl exception_vectors_table
exception_vectors_table:
    exception_vector_entry  sync_invalid_el1t           // Synchronous EL1t
    // todo: why cntv irq routes here?
    exception_vector_entry  el1_irq                     // IRQ EL1t
    exception_vector_entry  fiq_invalid_el1t            // FIQ EL1t
    exception_vector_entry  error_invalid_el1t          // Error EL1t

    // todo: handing sync_el1h
    exception_vector_entry  sync_invalid_el1h           // Synchronous EL1h
    exception_vector_entry  el1_irq                     // IRQ EL1h
    exception_vector_entry  fiq_invalid_el1h            // FIQ EL1h
    // todo: handing error_el1h
    exception_vector_entry  error_invalid_el1h          // Error EL1h

    exception_vector_entry  sync_invalid_el0_64         // Synchronous 64-bit EL0
    exception_vector_entry  irq_invalid_el0_64          // IRQ 64-bit EL0
    exception_vector_entry  fiq_invalid_el0_64          // FIQ 64-bit EL0
    exception_vector_entry  error_invalid_el0_64        // Error 64-bit EL0

    exception_vector_entry  sync_invalid_el0_32         // Synchronous 32-bit EL0
    exception_vector_entry  irq_invalid_el0_32          // IRQ 32-bit EL0
    exception_vector_entry  fiq_invalid_el0_32          // FIQ 32-bit EL0
    exception_vector_entry  error_invalid_el0_32        // Error 32-bit EL0

sync_invalid_el1t:
    handle_invalid_entry  %[SYNC_INVALID_EL1t]

irq_invalid_el1t:
    handle_invalid_entry  %[IRQ_INVALID_EL1t]

fiq_invalid_el1t:
    handle_invalid_entry  %[FIQ_INVALID_EL1t]

error_invalid_el1t:
    handle_invalid_entry  %[ERROR_INVALID_EL1t]

sync_invalid_el1h:
    handle_invalid_entry  %[SYNC_INVALID_EL1h]

fiq_invalid_el1h:
    handle_invalid_entry  %[FIQ_INVALID_EL1h]

error_invalid_el1h:
    handle_invalid_entry  %[ERROR_INVALID_EL1h]

sync_invalid_el0_64:
    handle_invalid_entry  %[SYNC_INVALID_EL0_64]

irq_invalid_el0_64:
    handle_invalid_entry  %[IRQ_INVALID_EL0_64]

fiq_invalid_el0_64:
    handle_invalid_entry  %[FIQ_INVALID_EL0_64]

error_invalid_el0_64:
    handle_invalid_entry  %[ERROR_INVALID_EL0_64]

sync_invalid_el0_32:
    handle_invalid_entry  %[SYNC_INVALID_EL0_32]

irq_invalid_el0_32:
    handle_invalid_entry  %[IRQ_INVALID_EL0_32]

fiq_invalid_el0_32:
    handle_invalid_entry  %[FIQ_INVALID_EL0_32]

error_invalid_el0_32:
    handle_invalid_entry  %[ERROR_INVALID_EL0_32]

el1_irq:
    kernel_entry
    bl  handle_el1_irq
    kernel_exit

.globl err_hang
err_hang:
    b err_hang
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


init_exception_vector_table_asm:
  adr    x0, exception_vectors_table        // load VBAR_EL1 with virtual
  msr    vbar_el1, x0 /* Vector Base Address Register (EL1) */
)"
               :
               : [S_FRAME_SIZE] "i"(S_FRAME_SIZE),

                 [SYNC_INVALID_EL1t] "i"(SYNC_INVALID_EL1t), [IRQ_INVALID_EL1t] "i"(IRQ_INVALID_EL1t),
                 [FIQ_INVALID_EL1t] "i"(FIQ_INVALID_EL1t), [ERROR_INVALID_EL1t] "i"(ERROR_INVALID_EL1t),

                 [SYNC_INVALID_EL1h] "i"(SYNC_INVALID_EL1h), [IRQ_INVALID_EL1h] "i"(IRQ_INVALID_EL1h),
                 [FIQ_INVALID_EL1h] "i"(FIQ_INVALID_EL1h), [ERROR_INVALID_EL1h] "i"(ERROR_INVALID_EL1h),

                 [SYNC_INVALID_EL0_64] "i"(SYNC_INVALID_EL0_64), [IRQ_INVALID_EL0_64] "i"(IRQ_INVALID_EL0_64),
                 [FIQ_INVALID_EL0_64] "i"(FIQ_INVALID_EL0_64), [ERROR_INVALID_EL0_64] "i"(ERROR_INVALID_EL0_64),

                 [SYNC_INVALID_EL0_32] "i"(SYNC_INVALID_EL0_32), [IRQ_INVALID_EL0_32] "i"(IRQ_INVALID_EL0_32),
                 [FIQ_INVALID_EL0_32] "i"(FIQ_INVALID_EL0_32), [ERROR_INVALID_EL0_32] "i"(ERROR_INVALID_EL0_32)
               :);
}
