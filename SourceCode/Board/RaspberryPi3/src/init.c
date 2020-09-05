#include "raspi3/external_timer.h"
#include "raspi3/interrupt_controller.h"
#include "raspi3/random.h"
#include "raspi3/timer_controller.h"
#include "raspi3/uart0.h"

int init_board() {
  init_uart0();

  init_random_generator();

//  routing_core0_cntv_to_core0_irq();
  routing_local_timer_to_core0_irq();

  //  init_lfb();
  //  // display a pixmap
  //  lfb_showpicture();
  return 0;
}
