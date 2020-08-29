#include "gpio.h"
#include "log.h"
#include "utils.h"

//#define TIMER_CS        (MMIO_BASE+0x00003000)
//#define TIMER_CLO       (MMIO_BASE+0x00003004)
//#define TIMER_CHI       (MMIO_BASE+0x00003008)
//#define TIMER_C0        (MMIO_BASE+0x0000300C)
//#define TIMER_C1        (MMIO_BASE+0x00003010)
//#define TIMER_C2        (MMIO_BASE+0x00003014)
//#define TIMER_C3        (MMIO_BASE+0x00003018)
//
//#define TIMER_CS_M0	(1 << 0)
//#define TIMER_CS_M1	(1 << 1)
//#define TIMER_CS_M2	(1 << 2)
//#define TIMER_CS_M3	(1 << 3)
//
//// See BCM2836 ARM-local peripherals at
//// https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2836/QA7_rev3.4.pdf
//
#define LPBASE 0x40000000
#define LTIMER_CTRL (LPBASE + 0x34)
#define LTIMER_CLR (LPBASE + 0x38)

#define LTIMER_CTRL_EN (1 << 28)
#define LTIMER_CTRL_INT_EN (1 << 29)
#define LTIMER_CTRL_VALUE (LTIMER_CTRL_EN | LTIMER_CTRL_INT_EN)

#define LTIMER_CLR_ACK (1 << 31)

const unsigned int interval = 200000;
unsigned int curVal = 0;
#define TIMER_CS (MMIO_BASE + 0x00003000)
#define TIMER_CLO (MMIO_BASE + 0x00003004)
#define TIMER_CHI (MMIO_BASE + 0x00003008)
#define TIMER_C0 (MMIO_BASE + 0x0000300C)
#define TIMER_C1 (MMIO_BASE + 0x00003010)
#define TIMER_C2 (MMIO_BASE + 0x00003014)
#define TIMER_C3 (MMIO_BASE + 0x00003018)

#define TIMER_CS_M0 (1 << 0)
#define TIMER_CS_M1 (1 << 1)
#define TIMER_CS_M2 (1 << 2)
#define TIMER_CS_M3 (1 << 3)

void timer_init(void) {
  curVal = get32(TIMER_CLO);
  curVal += interval;
  put32(TIMER_C1, curVal);
}

void handle_timer_irq(void) {
  curVal += interval;
  put32(TIMER_C1, curVal);
  put32(TIMER_CS, TIMER_CS_M1);
  log_i("Timer interrupt received\n\r");
}

void local_timer_init(void) { put32(LTIMER_CTRL, (interval | LTIMER_CTRL_VALUE)); }

void handle_local_timer_irq(void) {
  log_d("Timer interrupt received");
  put32(LTIMER_CLR, LTIMER_CLR_ACK);
}
