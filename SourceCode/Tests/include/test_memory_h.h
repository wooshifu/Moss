#pragma once

#include "memory.h"

TEST(should_io_write_and_read_works) {
  uint8_t a = 0;
  uint16_t b = 0;
  uint32_t c = 0;
  uint64_t d = 0;
  memory_write_8bits(&a, 1);
  ASSERT_EQ_UINT8(a, 1);
  memory_write_16bits(&b, 1);
  ASSERT_EQ_UINT16(b, 1);
  memory_write_32bits(&c, 1);
  ASSERT_EQ_UINT32(c, 1);
  memory_write_64bits(&d, 1);
  ASSERT_EQ_UINT32(d, 1);

  ASSERT_EQ_UINT8(memory_read_8bits(&a), 1);
  ASSERT_EQ_UINT16(memory_read_16bits(&b), 1);
  ASSERT_EQ_UINT32(memory_read_32bits(&c), 1);
  ASSERT_EQ_UINT64(memory_read_64bits(&d), 1);
}
