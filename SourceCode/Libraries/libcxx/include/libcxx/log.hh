#pragma once

#include "libstd/stdio.hh" // for printf

#ifndef CONFIG_LOG_LEVEL
#define CONFIG_LOG_LEVEL 3
#endif

// trace=1, verbose=2, debug=3, info=4, warning=5, error=6, fatal_error=7, panic=8
#ifndef LOG_LEVEL_TRACE
#define LOG_LEVEL_TRACE 1
#endif

#ifndef LOG_LEVEL_VERBOSE
#define LOG_LEVEL_VERBOSE 2
#endif

#ifndef LOG_LEVEL_DEBUG
#define LOG_LEVEL_DEBUG 3
#endif

#ifndef LOG_LEVEL_INFO
#define LOG_LEVEL_INFO 4
#endif

#ifndef LOG_LEVEL_WARNING
#define LOG_LEVEL_WARNING 5
#endif

#ifndef LOG_LEVEL_ERROR
#define LOG_LEVEL_ERROR 6
#endif

#ifndef LOG_LEVEL_FATAL_ERROR
#define LOG_LEVEL_FATAL_ERROR 7
#endif

#ifndef LOG_LEVEL_PANIC
#define LOG_LEVEL_PANIC 8
#endif

#ifndef __LOG_FORMAT
#if CONFIG_LOG_COLORFUL
#define __LOG_COLORFUL_NOCOLOR
#define __LOG_COLORFUL_RED               "\033[0;31m"
#define __LOG_COLORFUL_GREEN             "\033[0;32m"
#define __LOG_COLORFUL_YELLOW            "\033[0;33m"
#define __LOG_COLORFUL_BLUE              "\033[0;34m"
#define __LOG_COLORFUL_LIGHT_GRAY        "\033[0;37m"
#define __LOG_COLORFUL_PURPLE            "\033[0;35m"
#define __LOG_COLORFUL_CYAN              "\033[0;36m"
#define __LOG_COLORFUL_RESET             "\033[0m"
#define __LOG_FORMAT(prefix, fmt, color) color "[" #prefix " %s:%d] " fmt __LOG_COLORFUL_RESET "\n"
#else
#define __LOG_COLORFUL_NOCOLOR
#define __LOG_COLORFUL_RED
#define __LOG_COLORFUL_GREEN
#define __LOG_COLORFUL_YELLOW
#define __LOG_COLORFUL_BLUE
#define __LOG_COLORFUL_LIGHT_GRAY
#define __LOG_COLORFUL_PURPLE
#define __LOG_COLORFUL_CYAN
#define __LOG_COLORFUL_RESET
#define __LOG_FORMAT(prefix, fmt, color) "[" #prefix " %s:%d] " fmt "\n"
#endif
#endif

#ifndef log_t
#if CONFIG_LOG_LEVEL <= LOG_LEVEL_TRACE
#define log_t(fmt, ...) printf(__LOG_FORMAT(T, fmt, __LOG_COLORFUL_LIGHT_GRAY), __FILE_NAME__, __LINE__, ##__VA_ARGS__)
#else
#define log_t(fmt, ...)
#endif
#endif

#ifndef log_v
#if CONFIG_LOG_LEVEL <= LOG_LEVEL_VERBOSE
#define log_v(fmt, ...) printf(__LOG_FORMAT(V, fmt, __LOG_COLORFUL_LIGHT_GRAY), __FILE_NAME__, __LINE__, ##__VA_ARGS__)
#else
#define log_v(fmt, ...)
#endif
#endif

#ifndef log_d
#if CONFIG_LOG_LEVEL <= LOG_LEVEL_DEBUG
#define log_d(fmt, ...) printf(__LOG_FORMAT(D, fmt, __LOG_COLORFUL_LIGHT_GRAY), __FILE_NAME__, __LINE__, ##__VA_ARGS__)
#else
#define log_d(fmt, ...)
#endif
#endif

#ifndef log_i
#if CONFIG_LOG_LEVEL <= LOG_LEVEL_INFO
#define log_i(fmt, ...) printf(__LOG_FORMAT(I, fmt, __LOG_COLORFUL_GREEN), __FILE_NAME__, __LINE__, ##__VA_ARGS__)
#else
#define log_i(fmt, ...)
#endif
#endif

#ifndef log_w
#if CONFIG_LOG_LEVEL <= LOG_LEVEL_WARNING
#define log_w(fmt, ...) printf(__LOG_FORMAT(W, fmt, __LOG_COLORFUL_YELLOW), __FILE_NAME__, __LINE__, ##__VA_ARGS__)
#else
#define log_w(fmt, ...)
#endif
#endif

#ifndef log_e
#if CONFIG_LOG_LEVEL <= LOG_LEVEL_ERROR
#define log_e(fmt, ...) printf(__LOG_FORMAT(E, fmt, __LOG_COLORFUL_RED), __FILE_NAME__, __LINE__, ##__VA_ARGS__)
#else
#define log_e(fmt, ...)
#endif
#endif

#ifndef log_f
#if CONFIG_LOG_LEVEL <= LOG_LEVEL_FATAL_ERROR
#define log_f(fmt, ...) printf(__LOG_FORMAT(F, fmt, __LOG_COLORFUL_PURPLE), __FILE_NAME__, __LINE__, ##__VA_ARGS__)
#else
#define log_f(fmt, ...)
#endif
#endif

#ifndef log_p
#if CONFIG_LOG_LEVEL <= LOG_LEVEL_PANIC
#define log_p(fmt, ...) printf(__LOG_FORMAT(P, fmt, __LOG_COLORFUL_PURPLE), __FILE_NAME__, __LINE__, ##__VA_ARGS__)
#else
#define log_p(fmt, ...)
#endif
#endif
