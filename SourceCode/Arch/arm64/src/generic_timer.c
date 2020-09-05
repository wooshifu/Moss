#include "arch/generic_timer.h"
#include "libc/log.h"

#include <stdint.h>

void enable_cntv(void) {
  uint64_t cntv_ctl = 1;
  asm volatile("msr cntv_ctl_el0, %0" ::"r"(cntv_ctl));
}

void disable_cntv(void) {
  uint64_t cntv_ctl = 0;
  asm volatile("msr cntv_ctl_el0, %0" ::"r"(cntv_ctl));
}

uint64_t read_cntvct(void) {
  uint64_t val = 0;
  asm volatile("mrs %0, cntvct_el0" : "=r"(val));
  return val;
}

uint32_t read_cntv_tval(void) {
  uint64_t val = 0;
  asm volatile("mrs %0, cntv_tval_el0" : "=r"(val));
  return val;
}

void write_cntv_tval(uint64_t val) { asm volatile("msr cntv_tval_el0, %0" ::"r"(val)); }

uint64_t read_cntfrq() {
  uint64_t val = 0;
  asm volatile("mrs %0, cntfrq_el0" : "=r"(val));
  return val;
}

void handle_generic_timer_irq() {
  log_d("handle_generic_timer_irq");
  write_cntv_tval(625000);
}
