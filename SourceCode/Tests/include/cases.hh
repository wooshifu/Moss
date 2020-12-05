#pragma once

#include "tests/test_lib.hh"

#include "test_memory_h.hh"
#include "test_string_h.hh"
#include "test_test_lib_h.hh"

using test_case_func = void (*)();

test_case_func test_cases[] = {
    // clang-format off
    should_io_write_and_read_works,
    should_mem_clean_works,
    should_memset_set_memory,
    should_strlen_return_correct_len,
    should_strchr_works,
    should_strrchr_works,
    should_test_lib_test_eq_works,
    // clang-format on
};
