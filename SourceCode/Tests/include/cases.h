#pragma once

#include "lib/test_lib.h"

// --- TEST CASES BEGIN ---
#include "test_io_h.h"
#include "test_string_h.h"
#include "test_test_lib_h.h"
// --- TEST CASES END ---

typedef void (*test_case_func)();

test_case_func test_cases[] = {
    // clang-format off
    //
    should_mem_clean_works,
    should_memset_set_memory,
    should_strlen_return_correct_len,
    should_strchr_works,
    should_strrchr_works,
    //
    should_test_lib_test_eq_works,
    //
    should_io_write_and_read_works,
    // clang-format on
};

void run_tests() {
  size_t test_count = sizeof(test_cases) / sizeof(test_cases[0]);
  for (unsigned int index = 0; index < test_count; ++index) {
    test_case_func test_case = test_cases[index];
    if (test_case != NULL) {
      test_case();
    }
  }
  log_i("%d test cases, %d passed, %d failed", __test_all_asserts, __test_success_asserts, __test_failed_asserts);
}
