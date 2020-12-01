#pragma once

#include "tests/test_lib.hpp"

#include "test_memory_h.hpp"
#include "test_string_h.hpp"
#include "test_test_lib_h.hpp"

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
