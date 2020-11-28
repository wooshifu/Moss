#include "arm64/asm.hpp"
#include "arm64/timer.hpp"
#include "libcxx/log.hpp"

#include "libcxx/types.hpp"

void enable_cntv() {
  u64 cntv_ctl = 1;
  ARM64_WRITE_SYSTEM_REGISTER(cntv_ctl_el0, cntv_ctl);
}

void disable_cntv() {
  u64 cntv_ctl = 0;
  ARM64_WRITE_SYSTEM_REGISTER(cntv_ctl_el0, cntv_ctl);
}

u64 read_cntvct() { return ARM64_READ_SYSTEM_REGISTER(cntvct_el0); }

u32 read_cntv_tval() { return ARM64_READ_SYSTEM_REGISTER(cntv_tval_el0); }

void write_cntv_tval(u64 val) { ARM64_WRITE_SYSTEM_REGISTER(cntv_tval_el0, val); }

u64 read_cntfrq() { return ARM64_READ_SYSTEM_REGISTER(cntfrq_el0); }

void handle_generic_timer_irq() {
  log_d("handle_generic_timer_irq");
  // todo: change value
  write_cntv_tval(62500000);
}
