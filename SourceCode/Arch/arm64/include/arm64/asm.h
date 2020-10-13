#pragma once

#include <stdint.h>

#ifndef ARM64_WRITE_SYSTEM_REGISTER
#define ARM64_WRITE_SYSTEM_REGISTER(reg, value) asm volatile("msr " #reg ", %0" ::"r"(value))
#endif

#ifndef ARM64_READ_SYSTEM_REGISTER
#define ARM64_READ_SYSTEM_REGISTER(reg)                                                                                                 \
  ({                                                                                                                   \
    uint64_t value;                                                                                                    \
    asm volatile("mrs %0, " #reg : "=r"(value));                                                                       \
    value;                                                                                                             \
  })
#endif
