#pragma once

#include "libc/stdio.h"
#include "libc/string.h"

#define LOG_LEVEL_VERBOSE 1
#define LOG_LEVEL_DEBUG 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_WARNING 4
#define LOG_LEVEL_ERROR 5
#define LOG_LEVEL_FATAL 6

#define DEFAULT_LOG_LEVEL LOG_LEVEL_VERBOSE

#ifndef LOG_LEVEL
#define LOG_LEVEL DEFAULT_LOG_LEVEL
#endif

#ifndef COLORFUL_LOG_OUTPUT
#define COLORFUL_LOG_OUTPUT 1
#endif

#if COLORFUL_LOG_OUTPUT
#define LOG_OUTPUT_COLOR_BLACK "\033[30m"
#define LOG_OUTPUT_COLOR_RED "\033[31m"
#define LOG_OUTPUT_COLOR_GREEN "\033[32m"
#define LOG_OUTPUT_COLOR_YELLOW "\033[33m"
#define LOG_OUTPUT_COLOR_BLUE "\033[34m"
#define LOG_OUTPUT_COLOR_MAGENTA "\033[35m"
#define LOG_OUTPUT_COLOR_CYAN "\033[36m"
#define LOG_OUTPUT_COLOR_WHITE "\033[37m"

#define LOG_OUTPUT_COLOR_BACKGROUND_BLACK "\033[40m"
#define LOG_OUTPUT_COLOR_BACKGROUND_RED "\033[41m"
#define LOG_OUTPUT_COLOR_BACKGROUND_GREEN "\033[42m"
#define LOG_OUTPUT_COLOR_BACKGROUND_YELLOW "\033[43m"
#define LOG_OUTPUT_COLOR_BACKGROUND_BLUE "\033[44m"
#define LOG_OUTPUT_COLOR_BACKGROUND_MAGENTA "\033[45m"
#define LOG_OUTPUT_COLOR_BACKGROUND_CYAN "\033[46m"
#define LOG_OUTPUT_COLOR_BACKGROUND_WHITE "\033[47m"

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

const char *__extract_file_name_from_path(const char *file_path);

#ifndef __CURRENT_FILE_NAME__
//#define __CURRENT_FILE_NAME__ __extract_file_name_from_path(__FILE__)
#define __CURRENT_FILE_NAME__ __FILE__
#endif

/// log verbose
void _log_v(const char *format, ...);

#if LOG_LEVEL <= LOG_LEVEL_VERBOSE
#define log_v(format, ...)                                                                                             \
  _log_v(LOG_OUTPUT_COLOR_BLUE "[VERBOSE %s:%d(%s)]: " format LOG_OUTPUT_COLOR_RESET "\n", __CURRENT_FILE_NAME__,      \
         __LINE__, __func__, ##__VA_ARGS__)
#else
#define log_v(format, ...)
#endif

/// log debug
void _log_d(const char *format, ...);

#if LOG_LEVEL <= LOG_LEVEL_DEBUG
#define log_d(format, ...)                                                                                             \
  _log_d(LOG_OUTPUT_COLOR_CYAN "[DEBUG %s:%d(%s)]: " format LOG_OUTPUT_COLOR_RESET "\n", __CURRENT_FILE_NAME__,        \
         __LINE__, __func__, ##__VA_ARGS__)
#else
#define log_d(format, ...)
#endif

/// log info
void _log_i(const char *format, ...);

#if LOG_LEVEL <= LOG_LEVEL_INFO
#define log_i(format, ...)                                                                                             \
  _log_i(LOG_OUTPUT_COLOR_WHITE "[INFO %s:%d(%s)]: " format LOG_OUTPUT_COLOR_RESET "\n", __CURRENT_FILE_NAME__,        \
         __LINE__, __func__, ##__VA_ARGS__)
#else
#define log_i(format, ...)
#endif

/// log warning
void _log_w(const char *format, ...);

#if LOG_LEVEL <= LOG_LEVEL_WARNING
#define log_w(format, ...)                                                                                             \
  _log_w(LOG_OUTPUT_COLOR_YELLOW "[WARN %s:%d(%s)]: " format LOG_OUTPUT_COLOR_RESET "\n", __CURRENT_FILE_NAME__,       \
         __LINE__, __func__, ##__VA_ARGS__)
#else
#define log_w(format, ...)
#endif

/// log error
void _log_e(const char *format, ...);

#if LOG_LEVEL <= LOG_LEVEL_ERROR
#define log_e(format, ...)                                                                                             \
  _log_e(LOG_OUTPUT_COLOR_RED "[ERROR] %s:%d(%s)]: " format LOG_OUTPUT_COLOR_RESET "\n", __CURRENT_FILE_NAME__,        \
         __LINE__, __func__, ##__VA_ARGS__)
#else
#define log_e(format, ...)
#endif

/// log fatal
void _log_f(const char *format, ...);

#if LOG_LEVEL <= LOG_LEVEL_FATAL
#define log_f(format, ...)                                                                                             \
  _log_f(LOG_OUTPUT_COLOR_MAGENTA "[FATAL] %s:%d(%s)]: " format LOG_OUTPUT_COLOR_RESET "\n", __CURRENT_FILE_NAME__,    \
         __LINE__, __func__, ##__VA_ARGS__)
#else
#define log_f(format, ...)
#endif
