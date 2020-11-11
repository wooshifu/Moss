#pragma once

#ifdef __GNUC__

using u8  = __UINT8_TYPE__;
using u16 = __UINT16_TYPE__;
using u32 = __UINT32_TYPE__;
using u64 = __UINT64_TYPE__;

using i8  = __INT8_TYPE__;
using i16 = __INT16_TYPE__;
using i32 = __INT32_TYPE__;
using i64 = __INT64_TYPE__;

using iptr   = __INTPTR_TYPE__;
using uptr   = __UINTPTR_TYPE__;
using usize  = __SIZE_TYPE__;
using size_t = usize;

using intmax_t  = __INTMAX_TYPE__;
using uintptr_t = uptr;
using ptrdiff_t = __PTRDIFF_TYPE__;

#ifdef __DBL_MAX__
constexpr double DBL_MAX = __DBL_MAX__;
static_assert(DBL_MAX == 1.79769313486231570814527423731704357e+308L, "unexpected DBL_MAX");
#else
#error "DBL_MAX not defined, define it here"
#endif

#else
#error "put ypur own int types header here. currently only gcc is supported"
#endif

#ifndef __cplusplus
#ifndef bool
typedef _Bool bool;
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
#endif
