#pragma once

using i8      = __INT8_TYPE__;
using u8      = __UINT8_TYPE__;
using int8_t  = i8;
using uint8_t = u8;
static_assert(sizeof(i8) == 1 && sizeof(u8) == 1 && sizeof(int8_t) == 1 && sizeof(uint8_t) == 1);

using i16      = __INT16_TYPE__;
using u16      = __UINT16_TYPE__;
using int16_t  = i16;
using uint16_t = u16;
static_assert(sizeof(i16) == 2 && sizeof(u16) == 2 && sizeof(int16_t) == 2 && sizeof(uint16_t) == 2);

using i32      = __INT32_TYPE__;
using u32      = __UINT32_TYPE__;
using int32_t  = i32;
using uint32_t = u32;
static_assert(sizeof(i32) == 4 && sizeof(u32) == 4 && sizeof(int32_t) == 4 && sizeof(uint32_t) == 4);

using i64      = __INT64_TYPE__;
using u64      = __UINT64_TYPE__;
using int64_t  = i64;
using uint64_t = u64;
static_assert(sizeof(i64) == 8 && sizeof(u64) == 8 && sizeof(int64_t) == 8 && sizeof(uint64_t) == 8);

using intmax_t  = __INTMAX_TYPE__;
using uintmax_t = __UINTMAX_TYPE__;
static_assert(sizeof(intmax_t) == 8 && sizeof(uintmax_t) == 8);

using uint      = unsigned int;

using uintptr_t = __UINTPTR_TYPE__;
static_assert(sizeof(uintptr_t) == 8);
using ptrdiff_t = __PTRDIFF_TYPE__;
static_assert(sizeof(ptrdiff_t) == 8);

using size_t = __SIZE_TYPE__;
static_assert(sizeof(size_t) == 8);

using ssize_t = signed long long;
static_assert(sizeof(ssize_t) == 8);

using vaddr_t = uintptr_t;
static_assert(sizeof(vaddr_t) == 8);

using paddr_t = uintptr_t;
static_assert(sizeof(paddr_t) == 8);

using addr_t = uintptr_t;
static_assert(sizeof(addr_t) == 8);

using pte_t = u64;
static_assert(sizeof(pte_t) == 8);
