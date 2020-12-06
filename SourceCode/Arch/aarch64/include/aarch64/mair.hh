#pragma once

#include "aarch64/asm.hh"
#include "libcxx/log.hh"

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
using mair = struct [[gnu::packed]] mair {
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

consteval u64 mair_to_u64(const mair mair) {
  return mair._0 << 0 | mair._1 << 8 | mair._2 << 16 | mair._3 << 24 | (u64)mair._4 << 32 | (u64)mair._5 << 40 |
         (u64)mair._6 << 48 | (u64)mair._7 << 56;
}

constexpr u64 mair_value = mair_to_u64({device_nGnRnE, device_nGnRE, device_GRE, normal_NC, normal, normal_WT, _6, _7});
static_assert(mair_value == 0XBBFF'440C'0400);

void setup_mair_el1() { ARM64_WRITE_SYSREG(mair_el1, mair_value); }
