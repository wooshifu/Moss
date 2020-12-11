#pragma once

#ifdef __GNUC__

using u8  = __UINT8_TYPE__;
using u16 = __UINT16_TYPE__;
using u32 = __UINT32_TYPE__;
using u64 = __UINT64_TYPE__;
static_assert(sizeof(u8) == 1, "u8 must be size of 1");
static_assert(sizeof(u16) == 2, "u8 must be size of 2");
static_assert(sizeof(u32) == 4, "u8 must be size of 4");
static_assert(sizeof(u64) == 8, "u8 must be size of 8");

using i8  = __INT8_TYPE__;
using i16 = __INT16_TYPE__;
using i32 = __INT32_TYPE__;
using i64 = __INT64_TYPE__;
static_assert(sizeof(i8) == 1, "u8 must be size of 1");
static_assert(sizeof(i16) == 2, "u8 must be size of 2");
static_assert(sizeof(i32) == 4, "u8 must be size of 4");
static_assert(sizeof(i64) == 8, "u8 must be size of 8");

using nullptr_t = decltype(nullptr);
using iptr      = __INTPTR_TYPE__;
using uptr      = __UINTPTR_TYPE__;
using usize     = __SIZE_TYPE__;
using size_t    = usize;
static_assert(sizeof(iptr) == 8, "iptr must be size of 8");
static_assert(sizeof(uptr) == 8, "uptr must be size of 8");
static_assert(sizeof(usize) == 8, "usize must be size of 8");
static_assert(sizeof(size_t) == 8, "size_t must be size of 8");

using intmax_t  = __INTMAX_TYPE__;
using uintptr_t = uptr;
using ptrdiff_t = __PTRDIFF_TYPE__;
static_assert(sizeof(intmax_t) == 8, "intmax_t must be size of 8");
static_assert(sizeof(uintptr_t) == 8, "uintptr_t must be size of 8");
static_assert(sizeof(ptrdiff_t) == 8, "ptrdiff_t must be size of 8");

#ifdef __DBL_MAX__
constexpr double DBL_MAX = __DBL_MAX__;
static_assert(DBL_MAX == 1.79769313486231570814527423731704357e+308L, "unexpected DBL_MAX");
#else
#error "DBL_MAX not defined, define it here"
#endif

#ifdef __FLT_MAX__

constexpr double FLT_MAX = __FLT_MAX__;
static_assert(FLT_MAX == 3.40282346638528859811704183484516925e+38F, "unexpected FLT_MAX");
#else
#error "FLT_MAX not defined, define it here"
#endif

#else
#error "put ypur own int types header here. currently only clang is supported"
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
