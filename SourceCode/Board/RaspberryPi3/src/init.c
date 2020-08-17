#include "lfb.h"
#include "random.h"
#include "uart0.h"

int init_board() {
  init_uart0();
//
//  init_random_generator();
//
//  init_lfb();
//  // display a pixmap
//  lfb_showpicture();
  return 0;
}
