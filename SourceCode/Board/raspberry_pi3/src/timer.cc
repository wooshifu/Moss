#include "aarch64/timer.hh"
#include "libcxx/log.hh"
#include "libcxx/memory.hh" // for memory_write_32bits, memory_read_32bits
#include "libcxx/types.hh"  // for u32, u64
#include "rpi3/mmio.hh"
#include "rpi3/rpi3.hh"  // for NS_MMIO, NS_RPI3
#include "rpi3/timer.hh" // for handle_local_timer_irq, read_core0_pen...

namespace NS_rpi3 {
  /* reload value, should be max 28 bits(268435455). 38.4MHz */
  // todo: change value
  constexpr u32 interval = 38400000;

#if 0
#define TIMER_CS  (NS_MMIO::BASE + 0x00003000)
#define TIMER_CLO (NS_MMIO::BASE + 0x00003004)
#define TIMER_CHI (NS_MMIO::BASE + 0x00003008)
#define TIMER_C0  (NS_MMIO::BASE + 0x0000300C)
#define TIMER_C1  (NS_MMIO::BASE + 0x00003010)
#define TIMER_C2  (NS_MMIO::BASE + 0x00003014)
#define TIMER_C3  (NS_MMIO::BASE + 0x00003018)

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

  void routing_local_timer_to_core0_irq() {
    memory_write_32bits((u32*)NS_mmio::LOCAL_INTERRUPT_ROUTING, NS_mmio::LOCAL_TIMER_INTERRUPT_ROUTING_TO_CORE0_IRQ);
    memory_write_32bits((u32*)NS_mmio::LOCAL_TIMER_CONTROL, (interval | NS_mmio::LOCAL_TIMER_CONTROL_VALUE));
  }

  static u64 elapsed_seconds = 0;
  void handle_local_timer_irq() {
    ++elapsed_seconds;
    //  log_d("current elapsed seconds:%lu", elapsed_seconds);
    memory_write_32bits((u32*)NS_mmio::LOCAL_TIMER_CLEAR, NS_mmio::LOCAL_TIMER_CLEAR_ACK);
  }

  void routing_core0_cntv_to_core0_irq() {
    memory_write_32bits((u32*)NS_mmio::CORE0_TIMER_INTERRUPT_CONTROL, NS_mmio::COREn_CNTV_IRQ_INTERRUPT_ENABLED);
  }

  /// frequency, eg 62500000 Hz/second
  static u64 counter_frequency = 0;
  void init_generic_timer() {
    routing_core0_cntv_to_core0_irq();

    counter_frequency = read_cntfrq_el0();
    log_d("counter frequency el0: %lu", counter_frequency);
    write_cntv_tval_el0(counter_frequency);
    enable_cntv_el0();
    log_d("generic timer initialized");
  }

  void handle_generic_timer_irq() {
    static u64 count = 0;
    log_d("handle_generic_timer_irq %ld", count++);
    write_cntv_tval_el0(counter_frequency);
  }
} // namespace NS_rpi3
