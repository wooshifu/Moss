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

#ifndef __va_list__
using va_list = __builtin_va_list;
#endif
