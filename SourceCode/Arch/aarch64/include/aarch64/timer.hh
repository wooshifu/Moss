#pragma once

#include "aarch64/asm.hh"
#include "libcxx/types.hh"

inline void enable_cntv_el0() {
  constexpr u64 cntv_ctl = 1;
  ARM64_WRITE_SYSREG(cntv_ctl_el0, cntv_ctl);
}

inline void disable_cntv_el0() {
  constexpr u64 cntv_ctl = 0;
  ARM64_WRITE_SYSREG(cntv_ctl_el0, cntv_ctl);
}

inline u64 read_cntvct_el0() { return ARM64_READ_SYSREG(cntvct_el0); }

inline u32 read_cntv_tval_el0() { return ARM64_READ_SYSREG(cntv_tval_el0); }

inline void write_cntv_tval_el0(u64 val) { ARM64_WRITE_SYSREG(cntv_tval_el0, val); }

inline u64 read_cntfrq_el0() { return ARM64_READ_SYSREG(cntfrq_el0); }
