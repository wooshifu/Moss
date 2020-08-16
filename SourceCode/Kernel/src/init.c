#include "el.h"
#include "halt.h"
#include "lfb.h"
#include "log.h"
#include "logo.h"
#include "mailbox.h"
#include "power.h"
#include "random.h"
#include "uart.h"

extern void uart0_init();
extern void uart0_puts(char *s);

void kernel_main(void) {
  init_uart();

  print_moss_logo();

  log_i("current el: %d", get_current_el());
  mailbox_get_serial_number();

  uart0_init();
  uart0_puts("Hello From Uart0\n");

  rand_init();
  log_i("random: %u", rand(0, 100));

  lfb_init();
  // display a pixmap
  lfb_showpicture();
}
