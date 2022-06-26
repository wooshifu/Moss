#pragma once

constexpr double DBL_MAX = __DBL_MAX__;
static_assert(DBL_MAX == 1.7976931348623157e+308);

constexpr auto INT_MAX = __INT_MAX__;
static_assert(INT_MAX == 0x7fff'ffff);
