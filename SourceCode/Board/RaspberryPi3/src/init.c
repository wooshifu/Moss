#include "rpi3/external_timer.h"
#include "rpi3/random.h"
#include "rpi3/timer_controller.h"
#include "rpi3/uart0.h"

int init_board() {
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
