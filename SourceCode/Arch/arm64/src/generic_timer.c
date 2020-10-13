#include "arm64/asm.h"
#include "arm64/generic_timer.h"
#include "libc/log.h"

#include <stdint.h>

void enable_cntv(void) {
  uint64_t cntv_ctl = 1;
  ARM64_WRITE_SYSTEM_REGISTER(cntv_ctl_el0, cntv_ctl);
}

void disable_cntv(void) {
  uint64_t cntv_ctl = 0;
  ARM64_WRITE_SYSTEM_REGISTER(cntv_ctl_el0, cntv_ctl);
}

uint64_t read_cntvct(void) { return ARM64_READ_SYSTEM_REGISTER(cntvct_el0); }

uint32_t read_cntv_tval(void) { return ARM64_READ_SYSTEM_REGISTER(cntv_tval_el0); }

void write_cntv_tval(uint64_t val) { ARM64_WRITE_SYSTEM_REGISTER(cntv_tval_el0, val); }

uint64_t read_cntfrq() { return ARM64_READ_SYSTEM_REGISTER(cntfrq_el0); }

void handle_generic_timer_irq() {
  log_d("handle_generic_timer_irq");
  // todo: change value
  write_cntv_tval(62500000);
}
