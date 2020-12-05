#pragma once

#include "aarch64/asm.hpp"
#include "libcxx/log.hpp"

enum mair_field_value_index {
  device_nGnRnE_index = 0UL,
  device_nGnRE_index  = 1UL,
  device_GRE_index    = 2UL,
  normal_NC_index     = 3UL,
  normal_index        = 4UL,
  normal_WT_index     = 5UL,
  _6_index            = 6UL,
  _7_index            = 7UL
};

enum mair_field_value {
  device_nGnRnE = 0x00UL,
  device_nGnRE  = 0x04UL,
  device_GRE    = 0x0cUL,
  normal_NC     = 0x44UL,
  normal        = 0xffUL,
  normal_WT     = 0xbbUL,
  _6            = 0x00UL,
  _7            = 0x00UL
};

/// Memory Attribute Indirection Registers
struct [[gnu::packed]] mair {
  enum mair_field_value _0 : 8;
  enum mair_field_value _1 : 8;
  enum mair_field_value _2 : 8;
  enum mair_field_value _3 : 8;
  enum mair_field_value _4 : 8;
  enum mair_field_value _5 : 8;
  enum mair_field_value _6 : 8; /// not used
  enum mair_field_value _7 : 8; /// not used
};
static_assert(sizeof(struct mair) == 8, "sizeof mair must be 8");

constexpr const struct mair _mair_value = {._0 = device_nGnRnE,
                                     ._1 = device_nGnRE,
                                     ._2 = device_GRE,
                                     ._3 = normal_NC,
                                     ._4 = normal,
                                     ._5 = normal_WT,
                                     ._6 = _6,
                                     ._7 = _7};
constinit const u64 mair_value = _mair_value._0 << 0 | _mair_value._1 << 8 | _mair_value._2 << 16 | _mair_value._3 << 24 |
                           (u64)_mair_value._4 << 32 | (u64)_mair_value._5 << 40 | (u64)_mair_value._6 << 48 |
                           (u64)_mair_value._7 << 56;
static_assert(mair_value == 0XBBFF'440C'0400);

void setup_mair_el1() { ARM64_WRITE_SYSREG(mair_el1, mair_value); }
