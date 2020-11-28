#pragma once

#include "libcxx/types.hpp"

#ifndef ARM64_WRITE_SYSTEM_REGISTER
#define ARM64_WRITE_SYSTEM_REGISTER(register_name, value) asm volatile("msr " #register_name ", %0" ::"r"(value))
#endif

#ifndef ARM64_READ_SYSTEM_REGISTER
#define ARM64_READ_SYSTEM_REGISTER(register_name)                                                                      \
  ({                                                                                                                   \
    u64 value;                                                                                                         \
    asm volatile("mrs %0, " #register_name : "=r"(value));                                                             \
    value;                                                                                                             \
  })
#endif
