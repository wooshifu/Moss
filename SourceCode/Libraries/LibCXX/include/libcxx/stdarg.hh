#pragma once

#if 0
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
#endif

using va_list = char*;
#define va_start(ap, parmn) (void)((ap) = (char*)(&(parmn) + 1))
#define va_end(ap)          (void)((ap) = 0)
#define va_arg(ap, type)    (((type*)((ap) = ((ap) + sizeof(type))))[-1])
