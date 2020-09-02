#include "raspi3/random.h"
#include "raspi3/timer_controller.h"
#include "raspi3/uart0.h"

int init_board() {
  init_uart0();

  init_random_generator();

  routing_core0cntv_to_core0irq();

  //  init_lfb();
  //  // display a pixmap
  //  lfb_showpicture();
  return 0;
}
