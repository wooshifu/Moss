#pragma once

// Virtual address where the kernel address space begins.
// Below this is the user address space.
constexpr unsigned long long KERNEL_ASPACE_BASE = 0xffff'0000'0000'0000UL;
constexpr unsigned long long KERNEL_ASPACE_SIZE = 0x0001'0000'0000'0000UL;

// Virtual address where the user-accessible address space begins.
// Below this is wholly inaccessible.
constexpr unsigned long long USER_ASPACE_BASE   = 0x0000'0000'0020'0000UL;
constexpr unsigned long long USER_ASPACE_SIZE   = (0xffff'ff00'0000UL - USER_ASPACE_BASE);
