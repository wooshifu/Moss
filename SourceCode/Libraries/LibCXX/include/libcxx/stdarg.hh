#pragma once

#ifndef va_start
#define va_start(v, l) __builtin_va_start(v, l)
#endif

#ifndef va_end
#define va_end(v) __builtin_va_end(v)
#endif

#ifndef va_arg
#define va_arg(v, l) __builtin_va_arg(v, l)
#endif

#ifndef va_copy
#define va_copy(destination, source) __builtin_va_copy(destination, source)
#endif

#ifndef __va_list__
using va_list = __builtin_va_list;

// aarch64 va_list impl
// see https://developer.arm.com/documentation/ihi0055/latest/
// typedef struct  va_list {
//  void * stack; // next stack param
//  void * gr_top; // end of GP arg reg save area
//  void * vr_top; // end of FP/SIMD arg reg save area
//  int gr_offs; // offset from  gr_top to next GP register arg
//  int vr_offs; // offset from  vr_top to next FP/SIMD register arg
// } va_list;

#endif
