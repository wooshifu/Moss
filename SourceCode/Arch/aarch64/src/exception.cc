#include "aarch64/esr.hh"       // for esr_get_class_string
#include "aarch64/exception.hh" // for init_exception_vector_table_asm
#include "array"                // for array
#include "hal/init_hook.hh"     // for InitHookPriority, InitHookPriority::...
#include "libcxx/error_code.hh" // for KErrorCode, KErrorCode::OK
#include "libcxx/log.hh"        // for log_e
#include "libcxx/macro.hh"      // for extern_C
#include "libcxx/types.hh"      // for u64

KErrorCode init_exception_vector_table() {
  init_exception_vector_table_asm();
  return KErrorCode::OK;
}
AS_INIT_HOOK_PRE_KERNEL_MAIN(init_exception_vector_table, InitHookPriority::HIGHEST);

constexpr std::array entry_error_messages{
    "SYNC_INVALID_EL1t",   "IRQ_INVALID_EL1t",   "FIQ_INVALID_EL1t",   "ERROR_INVALID_EL1T",
    "SYNC_INVALID_EL1h",   "IRQ_INVALID_EL1h",   "FIQ_INVALID_EL1h",   "ERROR_INVALID_EL1h",
    "SYNC_INVALID_EL0_64", "IRQ_INVALID_EL0_64", "FIQ_INVALID_EL0_64", "ERROR_INVALID_EL0_64",
    "SYNC_INVALID_EL0_32", "IRQ_INVALID_EL0_32", "FIQ_INVALID_EL0_32", "ERROR_INVALID_EL0_32"};

extern_C void show_invalid_entry_message(int type, u64 far_el1, u64 sp, u64 esr_el1, u64 elr_el1) {
  log_e("[%s] far_el1: 0x%lx, sp: 0x%lx, esr_el1: 0x%lx, elr_el1: 0x%lx, exception class: %s",
        entry_error_messages[type], far_el1, sp, esr_el1, elr_el1, esr_get_class_string(esr_el1));
}
