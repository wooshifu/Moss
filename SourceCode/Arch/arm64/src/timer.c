#include "gpio.h"
#include "log.h"
#include "memory.h"

/// See BCM2836 ARM-local peripherals at
/// https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2836/QA7_rev3.4.pdf
#define LOCAL_PERIPHERALS_BASE 0x40000000
#define LOCAL_TIMER_CONTROL (LOCAL_PERIPHERALS_BASE + 0x34)
#define LOCAL_TIMER_CLEAR (LOCAL_PERIPHERALS_BASE + 0x38)
#define LOCAL_INTERRUPT_ROUTING (LOCAL_PERIPHERALS_BASE + 0x24)
#define LOCAL_TIMER_INTERRUPT_ROUTING_TO_CORE0_IRQ 0b000

#define LOCAL_TIMER_CONTROL_ENABLED (1 << 28)
#define LOCAL_TIMER_CONTROL_INTERRUPT_ENABLED (1 << 29)
#define LOCAL_TIMER_CONTROL_VALUE (LOCAL_TIMER_CONTROL_ENABLED | LOCAL_TIMER_CONTROL_INTERRUPT_ENABLED)
#define LOCAL_TIMER_CLEAR_ACK (1 << 31)

/* 38.4MHz */
const unsigned int interval = 38400000;

#if 0
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
  curVal = memory_read_32bits((const uint32_t *)TIMER_CLO);
  curVal += interval;
  memory_write_32bits((uint32_t *)TIMER_C1, curVal);
}

void handle_timer_irq(void) {
  curVal += interval;
  memory_write_32bits((uint32_t *)TIMER_C1, curVal);
  memory_write_32bits((uint32_t *)TIMER_CS, TIMER_CS_M1);
  log_i("Timer interrupt received");
}
#endif

void local_timer_init(void) {
  memory_write_32bits((uint32_t *)LOCAL_INTERRUPT_ROUTING, LOCAL_TIMER_INTERRUPT_ROUTING_TO_CORE0_IRQ);
  memory_write_32bits((uint32_t *)LOCAL_TIMER_CONTROL, (interval | LOCAL_TIMER_CONTROL_VALUE));
}

void handle_local_timer_irq(void) {
  log_d("Timer interrupt received");
  memory_write_32bits((uint32_t *)LOCAL_TIMER_CLEAR, LOCAL_TIMER_CLEAR_ACK);
}
