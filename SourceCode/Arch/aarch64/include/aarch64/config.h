#pragma once

constexpr auto IS_64BIT                = 1;
constexpr auto ARCH_DEFAULT_STACK_SIZE = 4096;
constexpr auto SMP_MAX_CPUS            = 4;
constexpr auto SMP_CPU_CLUSTER_SHIFT   = 8;
constexpr auto SMP_CPU_ID_BITS         = 24;
constexpr auto KERNEL_ASPACE_BASE      = 0xffff000000000000;
// constexpr auto KERNEL_ASPACE_SIZE      = 0x0001000000000000;
// constexpr auto USER_ASPACE_BASE        = 0x0000000001000000;
// constexpr auto USER_ASPACE_SIZE        = 0x0000fffffe000000;
// constexpr auto ARCH_HAS_MMU            = 1;
constexpr auto KERNEL_BASE             = 0xffff000000000000;
