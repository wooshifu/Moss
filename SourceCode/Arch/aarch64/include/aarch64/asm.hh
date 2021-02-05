#pragma once

#include "libcxx/types.hh"

#ifndef ARM64_WRITE_SYSREG
#define ARM64_WRITE_SYSREG(register_name, value) asm volatile("msr " #register_name ", %0" ::"r"(value))
#endif

// MRS Move to Register from Special Register
//
// moves the value from the selected special-purpose register into a general-purpose register.
#ifndef ARM64_READ_SYSREG
#define ARM64_READ_SYSREG(register_name)                                                                               \
  ({                                                                                                                   \
    u64 value;                                                                                                         \
    asm volatile("mrs %0, " #register_name : "=r"(value));                                                             \
    value;                                                                                                             \
  })
#endif
