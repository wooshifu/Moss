#pragma once

#include "aarch64/mmu.hh"

constexpr auto ROUNDUP(auto a, auto b) { return (a + (b - 1)) & ~(b - 1); }
constexpr auto ROUNDDOWN(auto a, auto b) { return a & ~(b - 1); }

constexpr auto ALIGN(auto a, auto b) { return ROUNDUP(a, b); }
constexpr auto IS_ALIGNED(auto a, auto b) {
  return !(reinterpret_cast<uintptr_t>(a) & (reinterpret_cast<uintptr_t>(b) - 1));
}

constexpr auto PAGE_ALIGN(auto x) { return ALIGN(x, PAGE_SIZE); }
constexpr auto ROUNDUP_PAGE_SIZE(auto x) { return ROUNDUP(x, PAGE_SIZE); }
constexpr auto IS_PAGE_ALIGNED(auto x) { return IS_ALIGNED(x, PAGE_SIZE); }
