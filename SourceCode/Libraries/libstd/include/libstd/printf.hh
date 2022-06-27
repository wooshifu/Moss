#pragma once

#include "libcxx/types.hh" // for size_t
#include "stdarg.hh"       // for va_list

/* printf engine that parses the format string and generates output */
/* function pointer to pass the printf engine, called back during the formatting.
 * input is a string to output, length bytes to output,
 * return code is number of characters that would have been written, or error code (if negative)
 */
using printf_engine_output_func = int (*)(const char*, size_t, void*);

int printf_engine(printf_engine_output_func out, void* state, const char* fmt, va_list ap);
