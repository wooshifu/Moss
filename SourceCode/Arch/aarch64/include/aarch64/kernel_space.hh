//
// Created by shifu on 2021/10/17.
//

#pragma once

#pragma once

// Virtual address where the kernel address space begins.
// Below this is the user address space.
constexpr auto KERNEL_ASPACE_BASE = 0xffff'0000'0000'0000UL;
constexpr auto KERNEL_ASPACE_SIZE = 0x0001'0000'0000'0000UL;

// Virtual address where the user-accessible address space begins.
// Below this is wholly inaccessible.
constexpr auto USER_ASPACE_BASE   = 0x0000'0000'0100'0000UL;
constexpr auto USER_ASPACE_SIZE   = 0x0000'ffff'fe00'0000UL;
