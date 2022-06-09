#pragma once

using i8 = __INT8_TYPE__;
using u8 = __UINT8_TYPE__;
static_assert(sizeof(i8) == 1 && sizeof(u8) == 1);
using i16 = __INT16_TYPE__;
using u16 = __UINT16_TYPE__;
static_assert(sizeof(i16) == 2 && sizeof(u16) == 2);
using i32 = __INT32_TYPE__;
using u32 = __UINT32_TYPE__;
static_assert(sizeof(i32) == 4 && sizeof(u32) == 4);
using i64 = __INT64_TYPE__;
using u64 = __UINT64_TYPE__;
static_assert(sizeof(i64) == 8 && sizeof(u64) == 8);

using uintptr_t = __UINTPTR_TYPE__;
static_assert(sizeof(uintptr_t) == 8);

using size_t = __SIZE_TYPE__;
static_assert(sizeof(size_t) == 8);

using vaddr_t = uintptr_t;
static_assert(sizeof(vaddr_t) == 8);
using paddr_t = uintptr_t;
static_assert(sizeof(paddr_t) == 8);
using pte_t = u64;
static_assert(sizeof(pte_t) == 8);
