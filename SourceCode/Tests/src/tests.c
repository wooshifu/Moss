#include "cases.h"
#include "halt.h"

void before_run_tests() {}

void kernel_main_tests() {
  before_run_tests();

  run_tests();

  halt();
}
