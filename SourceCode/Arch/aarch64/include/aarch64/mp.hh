#pragma once

#include "kconfig.hh"      // for CONFIG_SMP_MAX_CPUS
#include "libcxx/attr.hh"  // for attr_aligned
#include "libcxx/types.hh" // for u32
#include "libhal/cpu.hh"   // for cpu_num_t

constexpr auto CONF_SMP_MAX_CPUS = CONFIG_SMP_MAX_CPUS;
