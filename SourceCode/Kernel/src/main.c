#include "hal/el.h"
#include "hal/init.h"
#include "hal/random.h"
#include "kernel/logo.h"
#include "libc/log.h"

#include "arch/generic_timer.h"

extern uint8_t __text_start;

uint64_t xx; // bss data test

void kernel_main(void) {
  log_d(">>>>>>>>> entered kernel main <<<<<<<<<");
  init_board();

  print_moss_logo();

  init_cpu();

  for (int i = 0; i < 1; ++i) {
    log_d("test random: %d", generate_random(0, 100));
  }

  log_i("current el: %llu", get_current_el());
  long long ago = 0x1234567812345678;
  long long ago1 = 0x1234567812345678;
  log_d("kernel main done");
  log_d("ago is %p", &ago);
  log_d("__text_start is %p", &__text_start);
  log_d("ago1 is %p", &ago1);
  xx = 123;
  log_d("xx is: %llu", xx);
  double x = 5.0;
  double y = 3.0;
  double z = x / y;
  log_d("5/3=%f", z);
  log_d("long long ago: %llx", ago);

  //  uint64_t cntfrq = read_cntfrq();
  //  log_d("counter frq: %llu", cntfrq);
  //  write_cntv_tval(cntfrq);
  //  uint32_t cntv_tval = read_cntv_tval();
  //  log_d("cntv_tval is: %d", cntv_tval);
  //
  //  routing_core0cntv_to_core0irq();
  //  enable_cntv();
  //  enable_irq();
  int count = 0;
  while (1) {
    count++;
    if (count % 10000000 == 0)
      log_d("main");
  };
}
