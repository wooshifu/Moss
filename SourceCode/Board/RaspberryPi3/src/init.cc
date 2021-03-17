#include "hal/init.hh"
#include "libcxx/error_code.hh" // for not_ok, KErrorCode, KErrorCode::OK
#include "libcxx/log.hh"
#include "rpi3/rpi3.hh" // for NS_rpi3
#include "rpi3/timer.hh"
#include "rpi3/uart0.hh"

namespace NS_rpi3 {
  int init_board() {
    init_uart0();

    // todo: init mmu
    // init_mmu();
//    log_i("MMU ENABLED!!!");
    //  *(u64 *)(104857500 + 8) = 0x1234;
    //  for (u64 i = 104857500; i < 104857600 - 8; ++i) {
    //    log_i("%llx: mmu read: %llx", i, *(u64 *)(i));
    //  }

    // init generic timer
    init_generic_timer();
    // enable local timer
    //  routing_local_timer_to_core0_irq();

    return 0;
  }
} // namespace NS_rpi3

KErrorCode init_board() {
  auto result = NS_rpi3::init_board();
  if (not_ok(result)) { log_f("post init board failed, code: %d", result); }
  return KErrorCode::OK;
}
