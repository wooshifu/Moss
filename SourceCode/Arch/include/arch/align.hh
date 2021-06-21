#pragma once

#include "arch/defines.hh"
#include "libcxx/types.hh"

constexpr inline auto round_up(auto a, auto b) { return (a + (b - 1)) & ~(b - 1); }
constexpr inline auto round_down(auto a, auto b) { return (a & ~(b - 1)); }

constexpr inline auto align(auto a, auto b) { return round_up(a, b); }
constexpr inline bool is_aligned(auto a, auto b) { return !((uintptr_t)a & (uintptr_t)b - 1); }
constexpr inline auto page_align(auto x) { return align(x, PAGE_SIZE); }
constexpr inline auto roundup_page_size(auto x) { return round_up(x, PAGE_SIZE); }
constexpr inline auto is_page_aligned(auto x) { return is_aligned(x, PAGE_SIZE); }
