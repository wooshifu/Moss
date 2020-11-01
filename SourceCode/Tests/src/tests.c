#include "cases.h"
#include "hal/halt.h"

void before_run_tests() {
  printf("\n\n");
  log_i("---log output self test---");
  log_v("test log output, %s", "Hello World");
  log_d("test log output, %s", "Hello World");
  log_i("test log output, %s", "Hello World");
  log_w("test log output, %s", "Hello World");
  log_e("test log output, %s", "Hello World");
  log_f("test log output, %s", "Hello World");
  log_trace;
  log_i("---log output self test---");
  printf("\n\n");
}

void kernel_main_tests() {
  before_run_tests();

  run_tests();

  halt();
}
