#pragma once

#if __clang__ // && __clang_major__ >= 10

using int8_t  = __INT8_TYPE__;
using int16_t = __INT16_TYPE__;
using int32_t = __INT32_TYPE__;
using int64_t = __INT64_TYPE__;
static_assert(sizeof(int8_t) == 1, "sizeof int8_t must be 1");
static_assert(sizeof(int16_t) == 2, "sizeof int16_t must be 2");
static_assert(sizeof(int32_t) == 4, "sizeof int32_t must be 4");
static_assert(sizeof(int64_t) == 8, "sizeof int64_t must be 8");

using intmax_t = __INTMAX_TYPE__;
using intptr_t = __INTPTR_TYPE__;
static_assert(sizeof(intmax_t) == 8, "sizeof intmax_t must be 8");
static_assert(sizeof(intptr_t) == 8, "sizeof intptr_t must be 8");

using int_fast8_t  = __INT_FAST8_TYPE__;
using int_fast16_t = __INT_FAST16_TYPE__;
using int_fast32_t = __INT_FAST32_TYPE__;
using int_fast64_t = __INT_FAST64_TYPE__;
static_assert(sizeof(int_fast8_t) == 1, "sizeof int_fast8_t must be 1");
static_assert(sizeof(int_fast16_t) == 2, "sizeof int_fast26_t must be 1");
static_assert(sizeof(int_fast32_t) == 4, "sizeof int_fast32_t must be 4");
static_assert(sizeof(int_fast64_t) == 8, "sizeof int_fast64_t must be 8");

using int_least8_t  = __INT_LEAST8_TYPE__;
using int_least16_t = __INT_LEAST16_TYPE__;
using int_least32_t = __INT_LEAST32_TYPE__;
using int_least64_t = __INT_LEAST64_TYPE__;
static_assert(sizeof(int_least8_t) == 1, "sizeof int_least8_t must be 1");
static_assert(sizeof(int_least16_t) == 2, "sizeof int_least16_t must be 2");
static_assert(sizeof(int_least32_t) == 4, "sizeof int_least32_t must be 4");
static_assert(sizeof(int_least64_t) == 8, "sizeof int_least64_t must be 8");

using ptrdiff_t = __PTRDIFF_TYPE__;
using size_t    = __SIZE_TYPE__;
static_assert(sizeof(ptrdiff_t) == 8, "sizeof ptrdiff_t must be 8");
static_assert(sizeof(size_t) == 8, "sizeof size_t must be 8");

using uint8_t  = __UINT8_TYPE__;
using uint16_t = __UINT16_TYPE__;
using uint32_t = __UINT32_TYPE__;
using uint64_t = __UINT64_TYPE__;
static_assert(sizeof(uint8_t) == 1, "sizeof uint8_t must be 1");
static_assert(sizeof(uint16_t) == 2, "sizeof uint16_t must be 2");
static_assert(sizeof(uint32_t) == 4, "sizeof uint32_t must be 4");
static_assert(sizeof(uint64_t) == 8, "sizeof uint64_t must be 8");

using uintmax_t = __UINTMAX_TYPE__;
using uintptr_t = __UINTPTR_TYPE__;
static_assert(sizeof(uintmax_t) == 8, "sizeof uintmax_t must be 8");
static_assert(sizeof(uintptr_t) == 8, "sizeof uintptr_t must be 8");

using uint_fast8_t  = __UINT_FAST8_TYPE__;
using uint_fast16_t = __UINT_FAST16_TYPE__;
using uint_fast32_t = __UINT_FAST32_TYPE__;
using uint_fast64_t = __UINT_FAST64_TYPE__;
static_assert(sizeof(uint_fast8_t) == 1, "sizeof uint_fast8_t must be 1");
static_assert(sizeof(uint_fast16_t) == 2, "sizeof uint_fast16_t must be 2");
static_assert(sizeof(uint_fast32_t) == 4, "sizeof uint_fast32_t must be 4");
static_assert(sizeof(uint_fast64_t) == 8, "sizeof uint_fast64_t must be 8");

using uint_least8_t  = __UINT_LEAST8_TYPE__;
using uint_least16_t = __UINT_LEAST16_TYPE__;
using uint_least32_t = __UINT_LEAST32_TYPE__;
using uint_least64_t = __UINT_LEAST64_TYPE__;
static_assert(sizeof(uint_least8_t) == 1, "sizeof uint_least8_t must be 1");
static_assert(sizeof(uint_least16_t) == 2, "sizeof uint_least16_t must be 2");
static_assert(sizeof(uint_least32_t) == 4, "sizeof uint_least32_t must be 4");
static_assert(sizeof(uint_least64_t) == 8, "sizeof uint_least64_t must be 8");

using wint_t = __WINT_TYPE__;
static_assert(sizeof(wint_t) == 4, "sizeof wint_t must be 4");

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
static_assert(sizeof(u8) == 1, "sizeof u8 must 1");
static_assert(sizeof(u16) == 2, "sizeof u16 must be 2");
static_assert(sizeof(u32) == 4, "sizeof u32 must be 4");
static_assert(sizeof(u64) == 8, "sizeof u64 must be 8");

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
static_assert(sizeof(i8) == 1, "sizeof i8 must be 1");
static_assert(sizeof(i16) == 2, "sizeof i16 must be 2");
static_assert(sizeof(i32) == 4, "sizeof i32 must be 4");
static_assert(sizeof(i64) == 8, "sizeof i64 must be 8");

using nullptr_t = decltype(nullptr);
using iptr      = intptr_t;
using uptr      = uintptr_t;
static_assert(sizeof(nullptr_t) == 8, "sizeof nullptr_t must be 8");
static_assert(sizeof(iptr) == 8, "sizeof iptr must be 8");
static_assert(sizeof(uptr) == 8, "sizeof uptr must be 8");

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

#ifndef force_cast
#define force_cast(type, value) reinterpret_cast<type>(value)
#endif
