#pragma once

#include "arm64/asm.hpp"
#include "libcxx/log.hpp"

enum MairFieldValueIndex {
  device_nGnRnE_index = 0UL,
  device_nGnRE_index  = 1UL,
  device_GRE_index    = 2UL,
  normal_NC_index     = 3UL,
  normal_index        = 4UL,
  normal_WT_index     = 5UL,
  _6_index            = 6UL,
  _7_index            = 7UL
};

enum MairFieldValue {
  device_nGnRnE = 0x00UL,
  device_nGnRE  = 0x04UL,
  device_GRE    = 0x0cUL,
  normal_NC     = 0x44UL,
  normal        = 0xffUL,
  normal_WT     = 0xbbUL,
  _6            = 0x00UL,
  _7            = 0x00UL
};

struct Mair {
  enum MairFieldValue _0 : 8;
  enum MairFieldValue _1 : 8;
  enum MairFieldValue _2 : 8;
  enum MairFieldValue _3 : 8;
  enum MairFieldValue _4 : 8;
  enum MairFieldValue _5 : 8;
  enum MairFieldValue _6 : 8; /// not used
  enum MairFieldValue _7 : 8; /// not used
} __attribute__((packed));
static_assert(sizeof(struct Mair) == 8, "sizeof mair must be 8");

constexpr struct Mair _MAIR_VALUE = {._0 = device_nGnRnE,
                                     ._1 = device_nGnRE,
                                     ._2 = device_GRE,
                                     ._3 = normal_NC,
                                     ._4 = normal,
                                     ._5 = normal_WT,
                                     ._6 = _6,
                                     ._7 = _7};
constexpr u64 MAIR_VALUE = _MAIR_VALUE._0 << 0 | _MAIR_VALUE._1 << 8 | _MAIR_VALUE._2 << 16 | _MAIR_VALUE._3 << 24 |
                           (u64)_MAIR_VALUE._4 << 32 | (u64)_MAIR_VALUE._5 << 40 | (u64)_MAIR_VALUE._6 << 48 |
                           (u64)_MAIR_VALUE._7 << 56;
static_assert(MAIR_VALUE == 0XBBFF'440C'0400);

void setupMairEl1() {
  log_d("set mair to: 0x%llx", MAIR_VALUE);
  ARM64_WRITE_SYSTEM_REGISTER(mair_el1, MAIR_VALUE);
  log_d("mair set to: 0x%llx", ARM64_READ_SYSTEM_REGISTER(mair_el1));
}
