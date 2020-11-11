#pragma once

#include "libc/log.hpp"
#include "libc/stdio.hpp"
#include "libc/string.hpp"
#include "libc/types.hpp"

u32 __test_all_asserts     = 0;
u32 __test_success_asserts = 0;
u32 __test_failed_asserts  = 0;

#if 0
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
#define __on_assert_func_success_i8(failed_expression, actual, expected)                                           \
  __on_assert_func_success_(i8, failed_expression, actual, expected, %hhi)

#define __on_assert_func_failed_i8(failed_expression, actual, expected)                                            \
  __on_assert_func_failed_(i8, failed_expression, actual, expected, %hhi)

#define __on_assert_func_success_u8(failed_expression, actual, expected)                                          \
  __on_assert_func_success_(u8, failed_expression, actual, expected, %hhu)

#define __on_assert_func_failed_u8(failed_expression, actual, expected)                                           \
  __on_assert_func_failed_(u8, failed_expression, actual, expected, %hhu)

#define __on_assert_func_success_i16(failed_expression, actual, expected)                                          \
  __on_assert_func_success_(i16, failed_expression, actual, expected, %hi)

#define __on_assert_func_failed_i16(failed_expression, actual, expected)                                           \
  __on_assert_func_failed_(i16, failed_expression, actual, expected, %hi)

#define __on_assert_func_success_u16(failed_expression, actual, expected)                                         \
  __on_assert_func_success_(u16, failed_expression, actual, expected, %hu)

#define __on_assert_func_failed_u16(failed_expression, actual, expected)                                          \
  __on_assert_func_failed_(u16, failed_expression, actual, expected, %hu)

#define __on_assert_func_success_i32(failed_expression, actual, expected)                                          \
  __on_assert_func_success_(i32, failed_expression, actual, expected, %li)

#define __on_assert_func_failed_i32(failed_expression, actual, expected)                                           \
  __on_assert_func_failed_(i32, failed_expression, actual, expected, %li)

#define __on_assert_func_success_u32(failed_expression, actual, expected)                                         \
  __on_assert_func_success_(u32, failed_expression, actual, expected, %lu)

#define __on_assert_func_failed_u32(failed_expression, actual, expected)                                          \
  __on_assert_func_failed_(u32, failed_expression, actual, expected, %lu)

#define __on_assert_func_success_i64(failed_expression, actual, expected)                                          \
  __on_assert_func_success_(i64, failed_expression, actual, expected, %lli)

#define __on_assert_func_failed_i64(failed_expression, actual, expected)                                           \
  __on_assert_func_failed_(i64, failed_expression, actual, expected, %lli)

#define __on_assert_func_success_u64(failed_expression, actual, expected)                                         \
  __on_assert_func_success_(u64, failed_expression, actual, expected, %llu)

#define __on_assert_func_failed_u64(failed_expression, actual, expected)                                          \
  __on_assert_func_failed_(u64, failed_expression, actual, expected, %llu)

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

#define ASSERT_EQ_CHAR(actual, expected) tests_assert_(u8, (actual) == (expected), actual, expected)

// __SIZE_WIDTH__ gcc defined macro
#if __SIZE_WIDTH__ == 32
#define ASSERT_EQ_SIZE_T(actual, expected) tests_assert_(u32, (actual) == (expected), actual, expected)
#elif __SIZE_WIDTH__ == 64
#define ASSERT_EQ_SIZE_T(actual, expected) tests_assert_(u64, (actual) == (expected), actual, expected)
#else
#error "macro __SIZE_WIDTH__ not defined"
#endif

#define ASSERT_EQ_INT8(actual, expected)    tests_assert_(i8, (actual) == (expected), actual, expected)
#define ASSERT_EQ_UINT8(actual, expected)   tests_assert_(u8, (actual) == (expected), actual, expected)
#define ASSERT_EQ_INT16(actual, expected)   tests_assert_(i16, (actual) == (expected), actual, expected)
#define ASSERT_EQ_UINT16(actual, expected)  tests_assert_(u16, (actual) == (expected), actual, expected)
#define ASSERT_EQ_INT32(actual, expected)   tests_assert_(i32, (actual) == (expected), actual, expected)
#define ASSERT_EQ_UINT32(actual, expected)  tests_assert_(u32, (actual) == (expected), actual, expected)
#define ASSERT_EQ_INT64(actual, expected)   tests_assert_(i64, (actual) == (expected), actual, expected)
#define ASSERT_EQ_UINT64(actual, expected)  tests_assert_(u64, (actual) == (expected), actual, expected)
#define ASSERT_EQ_INT(actual, expected)     tests_assert_(int, (actual) == (expected), actual, expected)
#define ASSERT_EQ_POINTER(actual, expected) tests_assert_(ptrdiff_t, (actual) == (expected), actual, expected)

/*
void assert_eq_char(char actual, char expected) { ASSERT_EQ_CHAR(actual, expected); }
void assert_eq_i8(i8 actual, i8 expected) { ASSERT_EQ_INT8(actual, expected); }
void assert_eq_u8(u8 actual, u8 expected) { ASSERT_EQ_UINT8(actual, expected); }
void assert_eq_i16(i16 actual, i16 expected) { ASSERT_EQ_INT16(actual, expected); }
void assert_eq_u16(u16 actual, u16 expected) { ASSERT_EQ_UINT16(actual, expected); }
void assert_eq_i32(i32 actual, i32 expected) { ASSERT_EQ_INT32(actual, expected); }
void assert_eq_u32(u32 actual, u32 expected) { ASSERT_EQ_UINT32(actual, expected); }
void assert_eq_i64(i64 actual, i64 expected) { ASSERT_EQ_INT64(actual, expected); }
void assert_eq_u64(u64 actual, u64 expected) { ASSERT_EQ_UINT64(actual, expected); }
void assert_eq_int(int actual, int expected) { ASSERT_EQ_INT(actual, expected); }
void assert_eq_pointer(const void* actual, const void* expected) { ASSERT_EQ_POINTER(actual, expected); }
void assert_eq_size_t(size_t actual, size_t expected) { ASSERT_EQ_SIZE_T(actual, expected); }
*/
#endif

#ifndef __TEST_NAME_CONCAT
#define __TEST_NAME_CONCAT(a, b) a##b
#endif

#ifndef TEST
#define TEST(test_func_name) void __TEST_NAME_CONCAT(, test_func_name)()
#endif

#ifndef __cplusplus
#define __format_specifier(x)                                                                                          \
  _Generic((x),                                                                                                        \
           const char* : "%p",                                                                                         \
           char*       : "%p",                                                                                         \
           void*       : "%p",                                                                                         \
           char        : "%c",                                                                                         \
           i8          : "%hhi",                                                                                       \
           u8          : "%hhu",                                                                                       \
           i16         : "%hi",                                                                                        \
           u16         : "%hu",                                                                                        \
           i32         : "%li",                                                                                        \
           u32         : "%lu",                                                                                        \
           i64         : "%lli",                                                                                       \
           u64         : "%llu"                                                                                        \
  )
#else
constexpr const char *__format_specifier(const char * /*unused*/) { return "%p"; }
constexpr const char *__format_specifier(char * /*unused*/) { return "%p"; }
constexpr const char *__format_specifier(void * /*unused*/) { return "%p"; }
constexpr const char *__format_specifier(char /*unused*/) { return "%c"; }
constexpr const char *__format_specifier(i8 /*unused*/) { return "%hhi"; }
constexpr const char *__format_specifier(u8 /*unused*/) { return "%hhu"; }
constexpr const char *__format_specifier(i16 /*unused*/) { return "%hi"; }
constexpr const char *__format_specifier(u16 /*unused*/) { return "%hu"; }
constexpr const char *__format_specifier(i32 /*unused*/) { return "%li"; }
constexpr const char *__format_specifier(u32 /*unused*/) { return "%lu"; }
constexpr const char *__format_specifier(i64 /*unused*/) { return "%lli"; }
constexpr const char *__format_specifier(u64 /*unused*/) { return "%llu"; }
#endif

#if LOG_OUTPUT_LABEL && LOG_OUTPUT_FUNCTION_NAME
#define ASSERT_EQ(actual, expected)                                                                                    \
  do {                                                                                                                 \
    char __test_log_format[256];                                                                                       \
    if ((actual) == (expected)) {                                                                                      \
      strcpy(__test_log_format, LOG_OUTPUT_COLOR_WHITE LOG_OUTPUT_LABEL_VALUE(INFO));                                  \
    } else {                                                                                                           \
      strcpy(__test_log_format, LOG_OUTPUT_COLOR_RED LOG_OUTPUT_LABEL_VALUE(INFO));                                    \
    }                                                                                                                  \
    strcat(__test_log_format, "expected: %s == %s, actual: ");                                                         \
    strcat(__test_log_format, __format_specifier(actual));                                                             \
    strcat(__test_log_format, " == ");                                                                                 \
    strcat(__test_log_format, __format_specifier(expected));                                                           \
    strcat(__test_log_format, LOG_OUTPUT_COLOR_RESET);                                                                 \
    strcat(__test_log_format, "\n");                                                                                   \
    /*printf("format is %s", __test_log_format);*/                                                                     \
                                                                                                                       \
    ++__test_all_asserts;                                                                                              \
    if ((actual) == (expected)) {                                                                                      \
      ++__test_success_asserts;                                                                                        \
    } else {                                                                                                           \
      ++__test_failed_asserts;                                                                                         \
    }                                                                                                                  \
    printf(__test_log_format, LOG_OUTPUT_FUNCTION_NAME_PLACE_HOLDER_VALUE, #actual, #expected, (actual), (expected));  \
  } while (0)
#else
#define ASSERT_EQ(actual, expected)                                                                                    \
  do {                                                                                                                 \
    char __test_log_format[256];                                                                                       \
    strcpy(__test_log_format, "expected: %s == %s, actual: ");                                                         \
    strcat(__test_log_format, __format_specifier(actual));                                                             \
    strcat(__test_log_format, " == ");                                                                                 \
    strcat(__test_log_format, __format_specifier(expected));                                                           \
    strcat(__test_log_format, "\n");                                                                                   \
                                                                                                                       \
    ++__test_all_asserts;                                                                                              \
    if ((actual) == (expected)) {                                                                                      \
      ++__test_success_asserts;                                                                                        \
    } else {                                                                                                           \
      ++__test_failed_asserts;                                                                                         \
    }                                                                                                                  \
    printf(__test_log_format, #actual, #expected, (actual), (expected));                                               \
  } while (0)
#endif
