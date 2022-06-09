#pragma once

#include "libcxx/compiler.hh"

constexpr auto SHIFT_4K  = 12;
constexpr auto SHIFT_16K = 14;
constexpr auto SHIFT_64K = 16;

consteval auto PAGE_SIZE_SHIFT() { return SHIFT_4K; }

constexpr auto USER_PAGE_SIZE_SHIFT = SHIFT_4K;

constexpr auto PAGE_SIZE            = 1UL << PAGE_SIZE_SHIFT();
constexpr auto USER_PAGE_SIZE       = 1UL << USER_PAGE_SIZE_SHIFT;
