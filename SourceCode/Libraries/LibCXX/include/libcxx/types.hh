#pragma once

// gcc compiler version 10 and above
#if __GNUC__

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

using ptrdiff_t = __PTRDIFF_TYPE__;
using size_t    = __SIZE_TYPE__;
using ssize_t   = size_t;
static_assert(sizeof(ptrdiff_t) == 8, "sizeof ptrdiff_t must be 8");
static_assert(sizeof(size_t) == 8, "sizeof size_t must be 8");
static_assert(sizeof(ssize_t) == 8, "sizeof ssize_t must be 8");

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

using uint = unsigned int;

using vaddr_t = uintptr_t;
using paddr_t = uintptr_t;

using cpu_mask_t = uint32_t;
using cpu_num_t  = uint32_t;

using zx_status_t = int32_t;
