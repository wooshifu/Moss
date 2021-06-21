#include "cases.hh"
#include "hal/halt.hh"
#include "libcxx/log.hh"
#include "libcxx/printf.hh"
#include "libcxx/types.hh"
#include "tests/test_lib.hh"

void run_tests() {
  size_t test_count = sizeof(test_cases) / sizeof(test_cases[0]);
  for (unsigned int index = 0; index < test_count; ++index) {
    test_case_func test_case = test_cases[index];
    if (test_case != nullptr) { test_case(); }
  }
  log_i("%d test cases, %d passed, %d failed", test::__test_all_asserts, test::__test_success_asserts,
        test::__test_failed_asserts);
}

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

extern "C" void kernel_main_tests() {
  before_run_tests();

  run_tests();

  halt();
}
