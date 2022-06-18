#pragma once

constexpr auto ARCH_DEFAULT_STACK_SIZE = 8192;

/* map 512GB at the base of the kernel.
 * this is the max that can be mapped with a single level 1 page table using 1GB pages.
 */
constexpr auto ARCH_PHYSMAP_SIZE       = 1UL << 39;
