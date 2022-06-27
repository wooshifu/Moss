#pragma once

#include "libcxx/attr.hh"  // for attr_printf
#include "libcxx/types.hh" // for size_t
#include "libio/io.hh"     // for FILE
#include "stdarg.hh"       // for va_list

attr_printf(1, 2) int printf(const char* fmt, ...);

extern FILE stdio_FILEs[];

#define stdin  (&stdio_FILEs[0])
#define stdout (&stdio_FILEs[1])
#define stderr (&stdio_FILEs[2])

attr_printf(2, 3) int sprintf(char* str, const char* fmt, ...);
attr_printf(3, 4) int snprintf(char* str, size_t len, const char* fmt, ...);
int vsprintf(char* str, const char* fmt, va_list ap);
int vsnprintf(char* str, size_t len, const char* fmt, va_list ap);
int _fprintf_output_func(const char* str, size_t len, void* state);
