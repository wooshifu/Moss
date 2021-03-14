#include "hal/interrupt.hh"
#include "libcxx/log.hh"
#include "libcxx/macro.hh"

extern_C void handle_el1_irq(void) {
  log_d("interrupt occurred");

  bool handled = board_handle_interrupt();
  if (handled) {
    log_d("interrupt handled by peripheral");
    return;
  }

  handled = cpu_handle_interrupt();
  if (handled) {
    log_d("interrupt handled by cpu");
    return;
  }

  log_e("unknown interrupt!!!");
}

bool cpu_handle_interrupt() {
  log_d("currently, cpu will not handle any interrupts");
  return false;
}
