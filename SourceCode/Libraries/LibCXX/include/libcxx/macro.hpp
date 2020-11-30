#pragma once

#include "libcxx/constants.hpp"

#ifdef __STDC_HOSTED__
static_assert(__STDC_HOSTED__ == 0, "must be freestanding c++ mode");
#endif

#ifndef __STRINGIFY__
#define __INTERNAL_STRINGIFY_HELPER__(x) #x
#define __STRINGIFY__(x)                 __INTERNAL_STRINGIFY_HELPER__(x)
#endif

#ifndef extern_C
#ifdef __cplusplus
#define extern_C extern "C"
#else
#define extern_C
#endif
#endif

#ifndef __FILE_AND__LINE__
#define __FILE_AND__LINE__ __FILE__ ":" __STRINGIFY__(__LINE__)
#endif

#ifndef __CURRENT_FILE_NAME__
#define __CURRENT_FILE_NAME__ compile_time_filename(__FILE_AND__LINE__, sizeof(__FILE_AND__LINE__) - 1)
#endif

#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif
