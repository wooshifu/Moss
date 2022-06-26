#pragma once

#include "feature.hh"      // for arm64_microarch
#include "hal/cpu.hh"      // for cpu_num_t
#include "kconfig.hh"      // for CONFIG_SMP_MAX_CPUS
#include "libcxx/attr.hh"  // for attr_aligned
#include "libcxx/types.hh" // for u32

using cpu_mask_t = u32;
// using cpu_num_t  = u32;

struct percpu {};

// Per cpu structure, pointed to by a fixed register while in kernel mode.
// Aligned on the maximum architectural cache line to avoid cache line sharing between cpus.
constexpr auto MAX_CACHE_LINE = 64;
struct attr_aligned(MAX_CACHE_LINE) arm64_percpu {
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

constexpr auto CONF_SMP_MAX_CPUS = CONFIG_SMP_MAX_CPUS;
