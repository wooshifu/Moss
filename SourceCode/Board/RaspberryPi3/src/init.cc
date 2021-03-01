#include "hal/init.hh"
#include "rpi3/namespaces.hh"
#include "rpi3/uart0.hh"

namespace NS_RPI3 {
  int init_board() {
    init_uart0();

    //  init_mmu();
    //  log_i("MMU ENABLED!!!");
    //  *(u64 *)(104857500 + 8) = 0x1234;
    //  for (u64 i = 104857500; i < 104857600 - 8; ++i) {
    //    log_i("%llx: mmu read: %llx", i, *(u64 *)(i));
    //  }

    /*
      NS_RPI3::init_random_generator();

      for (int i = 0; i < 1000; ++i) {
        auto v = NS_RPI3::generate_random(1, 100);
        log_i("random data: %u", v);
      }
    */

    // enable generic timer
    //  routing_core0_cntv_to_core0_irq();

    // enable local timer
    //  routing_local_timer_to_core0_irq();

    //  init_lfb();
    //  // display a pixmap
    //  lfb_showpicture();
    return 0;
  }
} // namespace NS_RPI3

int init_board() { return rpi3::init_board(); }
