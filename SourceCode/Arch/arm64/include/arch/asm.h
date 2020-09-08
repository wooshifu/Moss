#pragma once

#include <stdint.h>

#ifndef ARM64_MSR
#define ARM64_MSR(reg, value) asm volatile("msr " #reg ", %0" ::"r"(value))
#endif

#ifndef ARM64_MRS
#define ARM64_MRS(reg)                                                                                                 \
  ({                                                                                                                   \
    uint64_t value;                                                                                                    \
    asm volatile("mrs %0, " #reg : "=r"(value));                                                                       \
    value;                                                                                                             \
  })
#endif
