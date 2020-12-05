#pragma once

#include "libcxx/memory.hh"

TEST(should_io_write_and_read_works) {
  u8 a  = 0;
  u16 b = 0;
  u32 c = 0;
  u64 d = 0;
  memory_write_8bits(&a, 1);
  ASSERT_EQ(a, 1);
  memory_write_16bits(&b, 1);
  ASSERT_EQ(b, 1);
  memory_write_32bits(&c, 1);
  ASSERT_EQ(c, 1);
  memory_write_64bits(&d, 1);
  ASSERT_EQ(d, 1);

  ASSERT_EQ(memory_read_8bits(&a), 1);
  ASSERT_EQ(memory_read_16bits(&b), 1);
  ASSERT_EQ(memory_read_32bits(&c), 1);
  ASSERT_EQ(memory_read_64bits(&d), 1);
}
