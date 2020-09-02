#include "memory.h"
#include <stdint.h>

#define CORE0_TIMER_IRQCNTL 0x40000040
#define CORE0_IRQ_SOURCE 0x40000060

void routing_core0cntv_to_core0irq() { memory_write_32bits((uint32_t *)CORE0_TIMER_IRQCNTL, 0x08); }

uint32_t read_core0timer_pending() { return memory_read_32bits((const uint32_t *)CORE0_IRQ_SOURCE); }

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
  return (val);
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
