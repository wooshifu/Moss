#pragma once

#include "libcxx/macro.hh"
#include "libcxx/printf.hh"

#define LOG_LEVEL_VERBOSE 1
#define LOG_LEVEL_DEBUG   2
#define LOG_LEVEL_INFO    3
#define LOG_LEVEL_WARNING 4
#define LOG_LEVEL_ERROR   5
#define LOG_LEVEL_FATAL   6

#define DEFAULT_LOG_LEVEL 1

#ifndef LOG_LEVEL
#define LOG_LEVEL DEFAULT_LOG_LEVEL
#endif

#ifndef LOG_OUTPUT_ENABLE_COLORFUL
#define LOG_OUTPUT_ENABLE_COLORFUL 1
#endif

/// log filename, line number, function name or not
/// enable log format like [DEBUG finename.c:25(function_name)] or not
#ifndef LOG_OUTPUT_ENABLE_LABEL
#define LOG_OUTPUT_ENABLE_LABEL 1
#endif

#if LOG_OUTPUT_ENABLE_LABEL
#define LOG_OUTPUT_LABEL_PLACEHOLDER(value) "[" #value " %s(%s)] "
#define LOG_OUTPUT_LABEL_PLACEHOLDER_VALUE  __CURRENT_FILE_NAME__, __func__
#else
#define LOG_OUTPUT_LABEL_PLACEHOLDER(value)
#define LOG_OUTPUT_LABEL_PLACEHOLDER_VALUE
#endif

#if LOG_OUTPUT_ENABLE_COLORFUL
#define LOG_OUTPUT_COLOR_BLACK   "\033[30m"
#define LOG_OUTPUT_COLOR_RED     "\033[31m"
#define LOG_OUTPUT_COLOR_GREEN   "\033[32m"
#define LOG_OUTPUT_COLOR_YELLOW  "\033[33m"
#define LOG_OUTPUT_COLOR_BLUE    "\033[34m"
#define LOG_OUTPUT_COLOR_MAGENTA "\033[35m"
#define LOG_OUTPUT_COLOR_CYAN    "\033[36m"
#define LOG_OUTPUT_COLOR_WHITE   "\033[37m"

#define LOG_OUTPUT_COLOR_BACKGROUND_BLACK   "\033[40m"
#define LOG_OUTPUT_COLOR_BACKGROUND_RED     "\033[41m"
#define LOG_OUTPUT_COLOR_BACKGROUND_GREEN   "\033[42m"
#define LOG_OUTPUT_COLOR_BACKGROUND_YELLOW  "\033[43m"
#define LOG_OUTPUT_COLOR_BACKGROUND_BLUE    "\033[44m"
#define LOG_OUTPUT_COLOR_BACKGROUND_MAGENTA "\033[45m"
#define LOG_OUTPUT_COLOR_BACKGROUND_CYAN    "\033[46m"
#define LOG_OUTPUT_COLOR_BACKGROUND_WHITE   "\033[47m"

#define LOG_OUTPUT_COLOR_RESET "\033[0m"
#else
#define LOG_OUTPUT_COLOR_BLACK
#define LOG_OUTPUT_COLOR_RED
#define LOG_OUTPUT_COLOR_GREEN
#define LOG_OUTPUT_COLOR_YELLOW
#define LOG_OUTPUT_COLOR_BLUE
#define LOG_OUTPUT_COLOR_MAGENTA
#define LOG_OUTPUT_COLOR_CYAN
#define LOG_OUTPUT_COLOR_WHITE

#define LOG_OUTPUT_COLOR_BACKGROUND_BLACK
#define LOG_OUTPUT_COLOR_BACKGROUND_RED
#define LOG_OUTPUT_COLOR_BACKGROUND_GREEN
#define LOG_OUTPUT_COLOR_BACKGROUND_YELLOW
#define LOG_OUTPUT_COLOR_BACKGROUND_BLUE
#define LOG_OUTPUT_COLOR_BACKGROUND_MAGENTA
#define LOG_OUTPUT_COLOR_BACKGROUND_CYAN
#define LOG_OUTPUT_COLOR_BACKGROUND_WHITE

#define LOG_OUTPUT_COLOR_RESET
#endif

#ifndef _log_function
#define _log_function(format, ...) printf(format, ##__VA_ARGS__)
#endif

/// log verbose
#if LOG_LEVEL <= LOG_LEVEL_VERBOSE
#if LOG_OUTPUT_ENABLE_LABEL
#define log_v(format, ...)                                                                                             \
  _log_function(LOG_OUTPUT_COLOR_BLUE LOG_OUTPUT_LABEL_PLACEHOLDER(VERBOSE) format LOG_OUTPUT_COLOR_RESET "\n",        \
                LOG_OUTPUT_LABEL_PLACEHOLDER_VALUE, ##__VA_ARGS__)
#else
#define log_v(format, ...) _log_function(LOG_OUTPUT_COLOR_BLUE format LOG_OUTPUT_COLOR_RESET "\n", ##__VA_ARGS__)
#endif
#else
#define log_v(format, ...)
#endif

/// log debug
#if LOG_LEVEL <= LOG_LEVEL_DEBUG
#if LOG_OUTPUT_ENABLE_LABEL
#define log_d(format, ...)                                                                                             \
  _log_function(LOG_OUTPUT_COLOR_CYAN LOG_OUTPUT_LABEL_PLACEHOLDER(DEBUG) format LOG_OUTPUT_COLOR_RESET "\n",          \
                LOG_OUTPUT_LABEL_PLACEHOLDER_VALUE, ##__VA_ARGS__)
#else
#define log_d(format, ...) _log_function(LOG_OUTPUT_COLOR_CYAN format LOG_OUTPUT_COLOR_RESET "\n", ##__VA_ARGS__)
#endif
/// log trace
#define log_trace log_d("---TRACING LOG---")
#else
#define log_d(format, ...)
#define log_trace()
#endif

/// log info
#if LOG_LEVEL <= LOG_LEVEL_INFO
#if LOG_OUTPUT_ENABLE_LABEL
#define log_i(format, ...)                                                                                             \
  _log_function(LOG_OUTPUT_COLOR_WHITE LOG_OUTPUT_LABEL_PLACEHOLDER(INFO) format LOG_OUTPUT_COLOR_RESET "\n",          \
                LOG_OUTPUT_LABEL_PLACEHOLDER_VALUE, ##__VA_ARGS__)
#else
#define log_i(format, ...) _log_function(LOG_OUTPUT_COLOR_WHITE format LOG_OUTPUT_COLOR_RESET "\n", ##__VA_ARGS__)
#endif
#else
#define log_i(format, ...)
#endif

/// log warning
#if LOG_LEVEL <= LOG_LEVEL_WARNING
#if LOG_OUTPUT_ENABLE_LABEL
#define log_w(format, ...)                                                                                             \
  _log_function(LOG_OUTPUT_COLOR_YELLOW LOG_OUTPUT_LABEL_PLACEHOLDER(WARN) format LOG_OUTPUT_COLOR_RESET "\n",         \
                LOG_OUTPUT_LABEL_PLACEHOLDER_VALUE, ##__VA_ARGS__)
#else
#define log_w(format, ...) _log_function(LOG_OUTPUT_COLOR_YELLOW format LOG_OUTPUT_COLOR_RESET "\n", ##__VA_ARGS__)
#endif
#else
#define log_w(format, ...)
#endif

/// log error
#if LOG_LEVEL <= LOG_LEVEL_ERROR
#if LOG_OUTPUT_ENABLE_LABEL
#define log_e(format, ...)                                                                                             \
  _log_function(LOG_OUTPUT_COLOR_RED LOG_OUTPUT_LABEL_PLACEHOLDER(ERROR) format LOG_OUTPUT_COLOR_RESET "\n",           \
                LOG_OUTPUT_LABEL_PLACEHOLDER_VALUE, ##__VA_ARGS__)
#else
#define log_e(format, ...) _log_function(LOG_OUTPUT_COLOR_RED format LOG_OUTPUT_COLOR_RESET "\n", ##__VA_ARGS__)
#endif
#else
#define log_e(format, ...)
#endif

/// log fatal
#if LOG_LEVEL <= LOG_LEVEL_FATAL
#if LOG_OUTPUT_ENABLE_LABEL
#define log_f(format, ...)                                                                                             \
  _log_function(LOG_OUTPUT_COLOR_MAGENTA LOG_OUTPUT_LABEL_PLACEHOLDER(FATAL) format LOG_OUTPUT_COLOR_RESET "\n",       \
                LOG_OUTPUT_LABEL_PLACEHOLDER_VALUE, ##__VA_ARGS__)
#else
#define log_f(format, ...) _log_function(LOG_OUTPUT_COLOR_MAGENTA format LOG_OUTPUT_COLOR_RESET "\n", ##__VA_ARGS__)
#endif
#else
#define log_f(format, ...)
#endif
