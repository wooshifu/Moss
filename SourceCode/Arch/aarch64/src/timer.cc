#include "aarch64/asm.hh"
#include "aarch64/timer.hh"
#include "libcxx/log.hh"
#include "libcxx/types.hh"

void enable_cntv() {
  u64 cntv_ctl = 1;
  ARM64_WRITE_SYSREG(cntv_ctl_el0, cntv_ctl);
}

void disable_cntv() {
  u64 cntv_ctl = 0;
  ARM64_WRITE_SYSREG(cntv_ctl_el0, cntv_ctl);
}

u64 read_cntvct() { return ARM64_READ_SYSREG(cntvct_el0); }

u32 read_cntv_tval() { return ARM64_READ_SYSREG(cntv_tval_el0); }

void write_cntv_tval(u64 val) { ARM64_WRITE_SYSREG(cntv_tval_el0, val); }

u64 read_cntfrq() { return ARM64_READ_SYSREG(cntfrq_el0); }

static u64 count = 0;
void handle_generic_timer_irq() {
  log_d("handle_generic_timer_irq");
  // todo: change value
  ++count;
  if (count % 100000 == 0) {
    log_d("count: %lu", count);
  }
  write_cntv_tval(62500000);
}
