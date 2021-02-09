#pragma once

#include "libcxx/linux_string.hh"
#include "libcxx/log.hh"
#include "libcxx/stdio.hh"
#include "libcxx/string.hh"
#include "libcxx/types.hh"

namespace test {
u32 __test_all_asserts     = 0;
u32 __test_success_asserts = 0;
u32 __test_failed_asserts  = 0;
} // namespace test

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
template <typename T> constexpr const char* __format_specifier(T /*unused*/);
constexpr const char* __format_specifier(nullptr_t /*unused*/) { return "%p"; }
constexpr const char* __format_specifier(const char* /*unused*/) { return "%p"; }
constexpr const char* __format_specifier(char* /*unused*/) { return "%p"; }
constexpr const char* __format_specifier(void* /*unused*/) { return "%p"; }
constexpr const char* __format_specifier(char /*unused*/) { return "%c"; }
constexpr const char* __format_specifier(i8 /*unused*/) { return "%hhi"; }
constexpr const char* __format_specifier(u8 /*unused*/) { return "%hhu"; }
constexpr const char* __format_specifier(i16 /*unused*/) { return "%hi"; }
constexpr const char* __format_specifier(u16 /*unused*/) { return "%hu"; }
constexpr const char* __format_specifier(i32 /*unused*/) { return "%li"; }
constexpr const char* __format_specifier(u32 /*unused*/) { return "%lu"; }
constexpr const char* __format_specifier(i64 /*unused*/) { return "%lli"; }
constexpr const char* __format_specifier(u64 /*unused*/) { return "%llu"; }
#endif

#if LOG_OUTPUT_ENABLE_LABEL
#if LOG_OUTPUT_ENABLE_COLORFUL
#define __append_LOG_OUTPUT_COLOR_RESET_to__test_log_format strcat(__test_log_format, LOG_OUTPUT_COLOR_RESET);
#else
#define __append_LOG_OUTPUT_COLOR_RESET_to__test_log_format
#endif
#define ASSERT_EQ(actual, expected)                                                                                    \
  do {                                                                                                                 \
    char __test_log_format[256];                                                                                       \
    if ((actual) == (expected)) {                                                                                      \
      strcpy(__test_log_format, LOG_OUTPUT_COLOR_WHITE LOG_OUTPUT_LABEL_PLACEHOLDER(INFO));                            \
    } else {                                                                                                           \
      strcpy(__test_log_format, LOG_OUTPUT_COLOR_RED LOG_OUTPUT_LABEL_PLACEHOLDER(INFO));                              \
    }                                                                                                                  \
    strcat(__test_log_format, "expected: %s == %s, actual: ");                                                         \
    strcat(__test_log_format, __format_specifier(actual));                                                             \
    strcat(__test_log_format, " == ");                                                                                 \
    strcat(__test_log_format, __format_specifier(expected));                                                           \
    __append_LOG_OUTPUT_COLOR_RESET_to__test_log_format strcat(__test_log_format, "\n");                               \
    /*printf("format is %s", __test_log_format);*/                                                                     \
                                                                                                                       \
    ++test::__test_all_asserts;                                                                                        \
    if ((actual) == (expected)) {                                                                                      \
      ++test::__test_success_asserts;                                                                                  \
    } else {                                                                                                           \
      ++test::__test_failed_asserts;                                                                                   \
    }                                                                                                                  \
    printf(__test_log_format, LOG_OUTPUT_LABEL_PLACEHOLDER_VALUE, #actual, #expected, (actual), (expected));           \
  } while (0)
#else
#define ASSERT_EQ(actual, expected)                                                                                    \
  do {                                                                                                                 \
    char __test_log_format[256];                                                                                       \
    strcpy(__test_log_format, __FILE_AND_LINE__ " expected: %s == %s, actual: ");                                      \
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
