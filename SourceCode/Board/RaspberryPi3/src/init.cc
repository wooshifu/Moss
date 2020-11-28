#include "hal/init.hpp"
#include "libcxx/log.hpp"
#include "rpi3/random.hpp"
#include "rpi3/timer.hpp"
#include "rpi3/uart0.hpp"

int init_board() {
  init_mmu();
  log_i("MMU ENABLED!!!");
  //  *(u64 *)(104857500 + 8) = 0x1234;
  //  for (u64 i = 104857500; i < 104857600 - 8; ++i) {
  //    log_i("%llx: mmu read: %llx", i, *(u64 *)(i));
  //  }

  init_uart0();

  init_random_generator();

  // enable generic timer
  routing_core0_cntv_to_core0_irq();

  // enable local timer
  routing_local_timer_to_core0_irq();

  //  init_lfb();
  //  // display a pixmap
  //  lfb_showpicture();
  return 0;
}
