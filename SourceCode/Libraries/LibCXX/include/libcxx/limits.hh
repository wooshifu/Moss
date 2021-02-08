#pragma once

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

#ifdef __INT_MAX__
constexpr int INT_MAX = __INT_MAX__;
#endif
