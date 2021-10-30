//
// Created by shifu on 2021/10/17.
//

#pragma once

#include "libcxx/unit.hh"

// Virtual address where the kernel address space begins.
// Below this is the user address space.
constexpr auto KERNEL_SPACE_START = 0xffff'0000'0000'0000UL;
constexpr auto KERNEL_SPACE_END   = 0xffff'ffff'ffff'ffffUL;
constexpr auto KERNEL_SPACE_SIZE  = 0x0000'ffff'ffff'ffffUL;
static_assert(KERNEL_SPACE_SIZE + 1 == 256_TiB);
static_assert(KERNEL_SPACE_END - KERNEL_SPACE_START == KERNEL_SPACE_SIZE);

// Virtual address where the user-accessible address space begins.
// Below this is wholly inaccessible.
constexpr auto USER_ASPACE_START = 0x0000'0000'0100'0000UL;
constexpr auto USER_ASPACE_END   = 0x0000'ffff'fe00'0000UL;
