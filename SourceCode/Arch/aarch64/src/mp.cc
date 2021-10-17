//
// Created by shifu on 2021/10/17.
//

#include "aarch64/mp.hh"
#include "kconfig.hh"

arm64_percpu arm64_percpu_array[CONFIG_SMP_MAX_CPUS];
