#pragma once

#include "arm64/asm.h"
#include "libc/log.h"

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

struct mair {
  enum mair_field_value _0 : 8;
  enum mair_field_value _1 : 8;
  enum mair_field_value _2 : 8;
  enum mair_field_value _3 : 8;
  enum mair_field_value _4 : 8;
  enum mair_field_value _5 : 8;
  enum mair_field_value _6 : 8; /// not used
  enum mair_field_value _7 : 8; /// not used
} __attribute__((packed)) const mair_value = {._0 = device_nGnRnE,
                                              ._1 = device_nGnRE,
                                              ._2 = device_GRE,
                                              ._3 = normal_NC,
                                              ._4 = normal,
                                              ._5 = normal_WT,
                                              ._6 = _6,
                                              ._7 = _7};
_Static_assert(sizeof(struct mair) == 8, "sizeof mair must be 8");

void setup_mair_el1() {
  log_d("set mair to: 0x%llx", mair_value);
  ARM64_WRITE_SYSTEM_REGISTER(mair_el1, mair_value);
  log_d("mair set to: 0x%llx", ARM64_READ_SYSTEM_REGISTER(mair_el1));
}
