#pragma once
#include "libcxx/types.hh"

constexpr auto ESR_ELx_EC_UNKNOWN     = 0x00;
constexpr auto ESR_ELx_EC_WFx         = 0x01;
/* Unallocated EC: 0x02 */
constexpr auto ESR_ELx_EC_CP15_32     = 0x03;
constexpr auto ESR_ELx_EC_CP15_64     = 0x04;
constexpr auto ESR_ELx_EC_CP14_MR     = 0x05;
constexpr auto ESR_ELx_EC_CP14_LS     = 0x06;
constexpr auto ESR_ELx_EC_FP_ASIMD    = 0x07;
constexpr auto ESR_ELx_EC_CP10_ID     = 0x08 /* EL2 only */;
constexpr auto ESR_ELx_EC_PAC         = 0x09 /* EL2 and above */;
/* Unallocated EC: 0x0A - 0x0B */
constexpr auto ESR_ELx_EC_CP14_64     = 0x0C;
constexpr auto ESR_ELx_EC_BTI         = 0x0D;
constexpr auto ESR_ELx_EC_ILL         = 0x0E;
/* Unallocated EC: 0x0F - 0x10 */
constexpr auto ESR_ELx_EC_SVC32       = 0x11;
constexpr auto ESR_ELx_EC_HVC32       = 0x12 /* EL2 only */;
constexpr auto ESR_ELx_EC_SMC32       = 0x13 /* EL2 and above */;
/* Unallocated EC: 0x14 */
constexpr auto ESR_ELx_EC_SVC64       = 0x15;
constexpr auto ESR_ELx_EC_HVC64       = 0x16 /* EL2 and above */;
constexpr auto ESR_ELx_EC_SMC64       = 0x17 /* EL2 and above */;
constexpr auto ESR_ELx_EC_SYS64       = 0x18;
constexpr auto ESR_ELx_EC_SVE         = 0x19;
constexpr auto ESR_ELx_EC_ERET        = 0x1a /* EL2 only */;
/* Unallocated EC: 0x1b - 0x1E */
constexpr auto ESR_ELx_EC_IMP_DEF     = 0x1f /* EL3 only */;
constexpr auto ESR_ELx_EC_IABT_LOW    = 0x20;
constexpr auto ESR_ELx_EC_IABT_CUR    = 0x21;
constexpr auto ESR_ELx_EC_PC_ALIGN    = 0x22;
/* Unallocated EC: 0x23 */
constexpr auto ESR_ELx_EC_DABT_LOW    = 0x24;
constexpr auto ESR_ELx_EC_DABT_CUR    = 0x25;
constexpr auto ESR_ELx_EC_SP_ALIGN    = 0x26;
/* Unallocated EC: 0x27 */
constexpr auto ESR_ELx_EC_FP_EXC32    = 0x28;
/* Unallocated EC: 0x29 - 0x2B */
constexpr auto ESR_ELx_EC_FP_EXC64    = 0x2C;
/* Unallocated EC: 0x2D - 0x2E */
constexpr auto ESR_ELx_EC_SERROR      = 0x2F;
constexpr auto ESR_ELx_EC_BREAKPT_LOW = 0x30;
constexpr auto ESR_ELx_EC_BREAKPT_CUR = 0x31;
constexpr auto ESR_ELx_EC_SOFTSTP_LOW = 0x32;
constexpr auto ESR_ELx_EC_SOFTSTP_CUR = 0x33;
constexpr auto ESR_ELx_EC_WATCHPT_LOW = 0x34;
constexpr auto ESR_ELx_EC_WATCHPT_CUR = 0x35;
/* Unallocated EC: 0x36 - 0x37 */
constexpr auto ESR_ELx_EC_BKPT32      = 0x38;
/* Unallocated EC: 0x39 */
constexpr auto ESR_ELx_EC_VECTOR32    = 0x3A /* EL2 only */;
/* Unallocated EC: 0x3B */
constexpr auto ESR_ELx_EC_BRK64       = 0x3C;
/* Unallocated EC: 0x3D - 0x3F */
constexpr auto ESR_ELx_EC_MAX         = 0x3F;

constexpr auto ESR_ELx_EC_SHIFT       = 26;
constexpr auto ESR_ELx_EC_MASK        = 0x3FUL << ESR_ELx_EC_SHIFT;
constexpr auto ESR_ELx_EC(auto esr) { return (esr & ESR_ELx_EC_MASK) >> ESR_ELx_EC_SHIFT; }

static inline bool esr_is_data_abort(u32 esr) {
  const u32 ec = ESR_ELx_EC(esr);

  return ec == ESR_ELx_EC_DABT_LOW || ec == ESR_ELx_EC_DABT_CUR;
}

const char* esr_get_class_string(u32 esr);
