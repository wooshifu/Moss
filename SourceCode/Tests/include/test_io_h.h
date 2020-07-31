#pragma once

#include "io.h"

void should_io_write_and_read_works() {
  uint8_t x = 0;
  uint16_t y = 0;
  uint32_t z = 0;
  io_writeb(1, &x);
  ASSERT_EQ_UINT8(x, 1);
  io_writew(1, &y);
  ASSERT_EQ_UINT16(y, 1);
  io_writel(1, &z);
  ASSERT_EQ_UINT32(z, 1);

  ASSERT_EQ_UINT8(io_readb(&x), 1);
  ASSERT_EQ_UINT16(io_readw(&y), 1);
  ASSERT_EQ_UINT32(io_readl(&z), 1);
}
