#include "hal/el.h"
#include "hal/init.h"
#include "hal/random.h"
#include "log.h"
#include "logo.h"
extern uint8_t __text_start;

uint64_t xx;

void kernel_main(void) {
  log_d(">>>>>>>>> entered kernel main <<<<<<<<<");
  init_board();

  init_cpu();
  print_moss_logo();

  for (int i = 0; i < 10; ++i) {
    log_d("test random: %d", generate_random(0, 100));
  }

  log_i("current el: %d", get_current_el());
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
}
