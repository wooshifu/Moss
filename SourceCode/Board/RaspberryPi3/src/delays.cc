#include "libcxx/types.hpp"
#include "rpi3/mmio.hpp"

#define SYSTMR_LO ((volatile unsigned int *)(MMIO_BASE + 0x00003004))
#define SYSTMR_HI ((volatile unsigned int *)(MMIO_BASE + 0x00003008))

#if 0
/**
 * Wait N CPU cycles (ARM CPU only)
 */
void wait_cycles(unsigned int n) {
  if (n) {
    while (n--) {
      asm volatile("nop");
    }
  }
}

/**
 * Wait N microsec (ARM CPU only)
 */
void wait_msec(unsigned int n) {
  register unsigned long f;
  register unsigned long t;
  register unsigned long r;
  // get the current counter frequency
  asm volatile("mrs %0, cntfrq_el0" : "=r"(f));
  // read the current counter
  asm volatile("mrs %0, cntpct_el0" : "=r"(t));
  // calculate expire value for counter
  t += ((f / 1000) * n) / 1000;
  do {
    asm volatile("mrs %0, cntpct_el0" : "=r"(r));
  } while (r < t);
}

/**
 * Get System Timer's counter
 */
u64 get_system_timer() {
  u64 h = -1;
  u64 l = 0;
  // we must read MMIO area as two separate 32 bit reads
  h = *SYSTMR_HI;
  l = *SYSTMR_LO;
  // we have to repeat it if high word changed during read
  if (h != *SYSTMR_HI) {
    h = *SYSTMR_HI;
    l = *SYSTMR_LO;
  }
  // compose long int value
  return (h << 32) | l;
}

/**
 * Wait N microsec (with BCM System Timer)
 */
void wait_msec_st(unsigned int n) {
  unsigned long t = get_system_timer();
  // we must check if it's non-zero, because qemu does not emulate
  // system timer, and returning constant zero would mean infinite loop
  if (t) {
    while (get_system_timer() < t + n) {
    }
  }
}

#endif