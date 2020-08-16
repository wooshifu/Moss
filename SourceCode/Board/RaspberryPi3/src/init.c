#include "lfb.h"
#include "log.h"
#include "random.h"
#include "uart0.h"

int init_board() {
  uart0_init();

  rand_init();
  log_i("random: %u", rand(0, 100));

  lfb_init();
  // display a pixmap
  lfb_showpicture();
  return 0;
}
