#pragma once

#include <stddef.h>

#include "libc/log.h"
#include "libc/stdio.h"
#include "libc/string.h"

uint32_t __test_all_asserts = 0;
uint32_t __test_success_asserts = 0;
uint32_t __test_failed_asserts = 0;

#define __on_assert_func_success_(type, failed_expression, actual, expected, format_specifier)                         \
  do {                                                                                                                 \
    ++__test_success_asserts;                                                                                          \
    log_d("expected: %s, actual: " #format_specifier " == " #format_specifier, failed_expression, (type)(actual),      \
          (type)(expected));                                                                                           \
  } while (0)

#define __on_assert_func_failed_(type, failed_expression, actual, expected, format_specifier)                          \
  do {                                                                                                                 \
    ++__test_failed_asserts;                                                                                           \
    log_e("expected: %s, actual: " #format_specifier " == " #format_specifier, failed_expression, (type)(actual),      \
          (type)(expected));                                                                                           \
  } while (0)

#if 0
#define __on_assert_func_success_(type, failed_expression, actual, expected, format_specifier)                         \
  do {                                                                                                                 \
    ++__test_success_asserts;                                                                                          \
    log_d("expected: %s, actual: %" format_specifier " == %" format_specifier, failed_expression, (type)(actual),      \
          (type)(expected));                                                                                           \
  } while (0)

#define __on_assert_func_failed_(type, failed_expression, actual, expected, format_specifier)                          \
  do {                                                                                                                 \
    ++__test_failed_asserts;                                                                                           \
    log_e("expected: %s, actual: %" format_specifier " == %" format_specifier, failed_expression, (type)(actual),      \
          (type)(expected));                                                                                           \
  } while (0)
#endif

// clang-format off
#define __on_assert_func_success_int8_t(failed_expression, actual, expected)                                           \
  __on_assert_func_success_(int8_t, failed_expression, actual, expected, %hhi)

#define __on_assert_func_failed_int8_t(failed_expression, actual, expected)                                            \
  __on_assert_func_failed_(int8_t, failed_expression, actual, expected, %hhi)

#define __on_assert_func_success_uint8_t(failed_expression, actual, expected)                                          \
  __on_assert_func_success_(uint8_t, failed_expression, actual, expected, %hhu)

#define __on_assert_func_failed_uint8_t(failed_expression, actual, expected)                                           \
  __on_assert_func_failed_(uint8_t, failed_expression, actual, expected, %hhu)

#define __on_assert_func_success_int16_t(failed_expression, actual, expected)                                          \
  __on_assert_func_success_(int16_t, failed_expression, actual, expected, %hi)

#define __on_assert_func_failed_int16_t(failed_expression, actual, expected)                                           \
  __on_assert_func_failed_(int16_t, failed_expression, actual, expected, %hi)

#define __on_assert_func_success_uint16_t(failed_expression, actual, expected)                                         \
  __on_assert_func_success_(uint16_t, failed_expression, actual, expected, %hu)

#define __on_assert_func_failed_uint16_t(failed_expression, actual, expected)                                          \
  __on_assert_func_failed_(uint16_t, failed_expression, actual, expected, %hu)

#define __on_assert_func_success_int32_t(failed_expression, actual, expected)                                          \
  __on_assert_func_success_(int32_t, failed_expression, actual, expected, %li)

#define __on_assert_func_failed_int32_t(failed_expression, actual, expected)                                           \
  __on_assert_func_failed_(int32_t, failed_expression, actual, expected, %li)

#define __on_assert_func_success_uint32_t(failed_expression, actual, expected)                                         \
  __on_assert_func_success_(uint32_t, failed_expression, actual, expected, %lu)

#define __on_assert_func_failed_uint32_t(failed_expression, actual, expected)                                          \
  __on_assert_func_failed_(uint32_t, failed_expression, actual, expected, %lu)

#define __on_assert_func_success_int64_t(failed_expression, actual, expected)                                          \
  __on_assert_func_success_(int64_t, failed_expression, actual, expected, %lli)

#define __on_assert_func_failed_int64_t(failed_expression, actual, expected)                                           \
  __on_assert_func_failed_(int64_t, failed_expression, actual, expected, %lli)

#define __on_assert_func_success_uint64_t(failed_expression, actual, expected)                                         \
  __on_assert_func_success_(uint64_t, failed_expression, actual, expected, %llu)

#define __on_assert_func_failed_uint64_t(failed_expression, actual, expected)                                          \
  __on_assert_func_failed_(uint64_t, failed_expression, actual, expected, %llu)

#define __on_assert_func_success_int(failed_expression, actual, expected)                                              \
  __on_assert_func_success_(int, failed_expression, actual, expected, %d)

#define __on_assert_func_failed_int(failed_expression, actual, expected)                                               \
  __on_assert_func_failed_(int, failed_expression, actual, expected, %d)

#define __on_assert_func_success_ptrdiff_t(failed_expression, actual, expected)                                        \
  __on_assert_func_success_(ptrdiff_t, failed_expression, actual, expected, %p)

#define __on_assert_func_failed_ptrdiff_t(failed_expression, actual, expected)                                         \
  __on_assert_func_failed_(ptrdiff_t, failed_expression, actual, expected, %p)
// clang-format on

#define tests_assert_(type, condition, actual, expected)                                                               \
  do {                                                                                                                 \
    ++__test_all_asserts;                                                                                              \
    if ((condition)) {                                                                                                 \
      __on_assert_func_success_##type(#condition, actual, expected);                                                   \
    } else {                                                                                                           \
      __on_assert_func_failed_##type(#condition, actual, expected);                                                    \
    }                                                                                                                  \
  } while (0)

#define ASSERT_EQ_CHAR(actual, expected) tests_assert_(uint8_t, (actual) == (expected), actual, expected)

// __SIZE_WIDTH__ gcc defined macro
#if __SIZE_WIDTH__ == 32
#define ASSERT_EQ_SIZE_T(actual, expected) tests_assert_(uint32_t, (actual) == (expected), actual, expected)
#elif __SIZE_WIDTH__ == 64
#define ASSERT_EQ_SIZE_T(actual, expected) tests_assert_(uint64_t, (actual) == (expected), actual, expected)
#else
#error "macro __SIZE_WIDTH__ not defined"
#endif

#define ASSERT_EQ_INT8(actual, expected) tests_assert_(int8_t, (actual) == (expected), actual, expected)
#define ASSERT_EQ_UINT8(actual, expected) tests_assert_(uint8_t, (actual) == (expected), actual, expected)
#define ASSERT_EQ_INT16(actual, expected) tests_assert_(int16_t, (actual) == (expected), actual, expected)
#define ASSERT_EQ_UINT16(actual, expected) tests_assert_(uint16_t, (actual) == (expected), actual, expected)
#define ASSERT_EQ_INT32(actual, expected) tests_assert_(int32_t, (actual) == (expected), actual, expected)
#define ASSERT_EQ_UINT32(actual, expected) tests_assert_(uint32_t, (actual) == (expected), actual, expected)
#define ASSERT_EQ_INT64(actual, expected) tests_assert_(int64_t, (actual) == (expected), actual, expected)
#define ASSERT_EQ_UINT64(actual, expected) tests_assert_(uint64_t, (actual) == (expected), actual, expected)
#define ASSERT_EQ_INT(actual, expected) tests_assert_(int, (actual) == (expected), actual, expected)
#define ASSERT_EQ_POINTER(actual, expected) tests_assert_(ptrdiff_t, (actual) == (expected), actual, expected)

/*
void assert_eq_char(char actual, char expected) { ASSERT_EQ_CHAR(actual, expected); }
void assert_eq_int8_t(int8_t actual, int8_t expected) { ASSERT_EQ_INT8(actual, expected); }
void assert_eq_uint8_t(uint8_t actual, uint8_t expected) { ASSERT_EQ_UINT8(actual, expected); }
void assert_eq_int16_t(int16_t actual, int16_t expected) { ASSERT_EQ_INT16(actual, expected); }
void assert_eq_uint16_t(uint16_t actual, uint16_t expected) { ASSERT_EQ_UINT16(actual, expected); }
void assert_eq_int32_t(int32_t actual, int32_t expected) { ASSERT_EQ_INT32(actual, expected); }
void assert_eq_uint32_t(uint32_t actual, uint32_t expected) { ASSERT_EQ_UINT32(actual, expected); }
void assert_eq_int64_t(int64_t actual, int64_t expected) { ASSERT_EQ_INT64(actual, expected); }
void assert_eq_uint64_t(uint64_t actual, uint64_t expected) { ASSERT_EQ_UINT64(actual, expected); }
void assert_eq_int(int actual, int expected) { ASSERT_EQ_INT(actual, expected); }
void assert_eq_pointer(const void* actual, const void* expected) { ASSERT_EQ_POINTER(actual, expected); }
void assert_eq_size_t(size_t actual, size_t expected) { ASSERT_EQ_SIZE_T(actual, expected); }
*/

#ifndef __TEST_NAME_CONCAT
#define __TEST_NAME_CONCAT(a, b) a##b
#endif

#ifndef TEST
#define TEST(test_func_name) void __TEST_NAME_CONCAT(, test_func_name)()
#endif
