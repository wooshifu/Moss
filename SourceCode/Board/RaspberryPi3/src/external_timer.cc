#include "libc/log.h"
#include "libc/memory.h"
#include "rpi3/external_timer.h"
#include "rpi3/mmio.h"

/* reload value, should be max 28 bits(268435455). 38.4MHz */
// todo: change value
static const u32 interval = 38400000;

#if 0
#define TIMER_CS  (MMIO_BASE + 0x00003000)
#define TIMER_CLO (MMIO_BASE + 0x00003004)
#define TIMER_CHI (MMIO_BASE + 0x00003008)
#define TIMER_C0  (MMIO_BASE + 0x0000300C)
#define TIMER_C1  (MMIO_BASE + 0x00003010)
#define TIMER_C2  (MMIO_BASE + 0x00003014)
#define TIMER_C3  (MMIO_BASE + 0x00003018)

#define TIMER_CS_M0 (1 << 0)
#define TIMER_CS_M1 (1 << 1)
#define TIMER_CS_M2 (1 << 2)
#define TIMER_CS_M3 (1 << 3)

void timer_init(void) {
  curVal = memory_read_32bits((const u32 *)TIMER_CLO);
  curVal += interval;
  memory_write_32bits((u32 *)TIMER_C1, curVal);
}

void handle_timer_irq(void) {
  curVal += interval;
  memory_write_32bits((u32 *)TIMER_C1, curVal);
  memory_write_32bits((u32 *)TIMER_CS, TIMER_CS_M1);
  log_i("Timer interrupt received");
}
#endif

void routing_local_timer_to_core0_irq(void) {
  memory_write_32bits((u32 *)LOCAL_INTERRUPT_ROUTING, LOCAL_TIMER_INTERRUPT_ROUTING_TO_CORE0_IRQ);
  memory_write_32bits((u32 *)LOCAL_TIMER_CONTROL, (interval | LOCAL_TIMER_CONTROL_VALUE));
}

static u64 elapsed_seconds = 0;
void handle_local_timer_irq(void) {
  ++elapsed_seconds;
  log_d("current elapsed seconds:%llu", elapsed_seconds);
  memory_write_32bits((u32 *)LOCAL_TIMER_CLEAR, LOCAL_TIMER_CLEAR_ACK);
}
