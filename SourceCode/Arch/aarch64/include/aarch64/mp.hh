//
// Created by shifu on 2021/10/17.
//

#pragma once
// bits for mpidr register

#include "libcxx/types.hh"

constexpr auto MPIDR_AFF0_MASK  = 0xFFULL;
constexpr auto MPIDR_AFF0_SHIFT = 0;
constexpr auto MPIDR_AFF1_MASK  = 0xFFULL << 8;
constexpr auto MPIDR_AFF1_SHIFT = 8;
constexpr auto MPIDR_AFF2_MASK  = 0xFFULL << 16;
constexpr auto MPIDR_AFF2_SHIFT = 16;
constexpr auto MPIDR_AFF3_MASK  = 0xFFULL << 32;
constexpr auto MPIDR_AFF3_SHIFT = 32;

// construct a ARM MPID from cluster (AFF1) and cpu number (AFF0)
consteval auto ARM64_MPID(auto cluster, auto cpu) {
  return ((cluster << MPIDR_AFF1_SHIFT) & MPIDR_AFF1_MASK) | ((cpu << MPIDR_AFF0_SHIFT) & MPIDR_AFF0_MASK);
}

constexpr auto ARM64_MPIDR_MASK = MPIDR_AFF3_MASK | MPIDR_AFF2_MASK | MPIDR_AFF1_MASK | MPIDR_AFF0_MASK;

// TODO: add support for AFF2 and AFF3

// todo: percpu not defined???
struct percpu;
using cpu_num_t = u32;
enum arm64_microarch {
  UNKNOWN,

  ARM_CORTEX_A32,
  ARM_CORTEX_A35,
  ARM_CORTEX_A53,
  ARM_CORTEX_A55,
  ARM_CORTEX_A57,
  ARM_CORTEX_A65,
  ARM_CORTEX_A72,
  ARM_CORTEX_A73,
  ARM_CORTEX_A75,
  ARM_CORTEX_A76,
  ARM_CORTEX_A76AE,
  ARM_CORTEX_A77,
  ARM_CORTEX_A78,
  ARM_NEOVERSE_E1,
  ARM_NEOVERSE_N1,

  CAVIUM_CN88XX,
  CAVIUM_CN99XX,

  QEMU_TCG,
};
// Per cpu structure, pointed to by a fixed register while in kernel mode.
// Aligned on the maximum architectural cache line to avoid cache
// line sharing between cpus.
constexpr auto MAX_CACHE_LINE = 128;
struct [[gnu::aligned(MAX_CACHE_LINE)]] arm64_percpu {
  // cpu number
  cpu_num_t cpu_num;

  // Whether blocking is disallowed.  See arch_blocking_disallowed().
  u32 blocking_disallowed;

  // Number of spinlocks currently held.
  u32 num_spinlocks;

  // Microarchitecture of this cpu (ex: Cortex-A53)
  arm64_microarch microarch;

  // True if the branch predictor should be invalidated during context switch or on suspicious
  // entries to EL2 to mitigate Spectre V2 attacks.
  bool should_invalidate_bp_on_context_switch;

  // A pointer providing fast access to the high-level arch-agnostic per-cpu struct.
  percpu* high_level_percpu;
};
