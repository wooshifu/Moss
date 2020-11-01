#pragma once

#ifdef __GNUC__

typedef __UINT8_TYPE__ u8;
typedef __UINT16_TYPE__ u16;
typedef __UINT32_TYPE__ u32;
typedef __UINT64_TYPE__ u64;

typedef __INT8_TYPE__ i8;
typedef __INT16_TYPE__ i16;
typedef __INT32_TYPE__ i32;
typedef __INT64_TYPE__ i64;

typedef __INTPTR_TYPE__ iptr;
typedef __UINTPTR_TYPE__ uptr;
typedef u64 usize;
// typedef i64 isize;

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
