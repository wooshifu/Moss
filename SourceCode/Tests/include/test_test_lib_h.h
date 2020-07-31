#pragma once

#include "lib/test_lib.h"

void should_test_lib_test_eq_works() {
  int x = 1;
  int y = 1;
  ASSERT_EQ_INT(x, y);

  int8_t xi8 = 0x12;
  int8_t yi8 = 0x12;
  ASSERT_EQ_INT8(xi8, yi8);
  uint8_t xu8 = 0x21;
  uint8_t yu8 = 0x21;
  ASSERT_EQ_UINT8(xu8, yu8);

  int16_t xi16 = 0x1234;
  int16_t yi16 = 0x1234;
  ASSERT_EQ_INT16(xi16, yi16);
  uint16_t xu16 = 0x4321;
  uint16_t yu16 = 0x4321;
  ASSERT_EQ_UINT16(xu16, yu16);

  int32_t xi32 = 0x12345678;
  int32_t yi32 = 0x12345678;
  ASSERT_EQ_INT32(xi32, yi32);
  uint32_t xu32 = 0x87654321;
  uint32_t yu32 = 0x87654321;
  ASSERT_EQ_UINT32(xu32, yu32);

  int64_t xi64 = 0x1234567812345678;
  int64_t yi64 = 0x1234567812345678;
  ASSERT_EQ_INT64(xi64, yi64);
  uint64_t xu64 = 0x1234567812345678;
  uint64_t yu64 = 0x1234567812345678;
  ASSERT_EQ_UINT64(xu64, yu64);

  char xc = 'A';
  char yc = 'A';
  ASSERT_EQ_CHAR(xc, yc);

  size_t xs = 1;
  size_t ys = 1;
  ASSERT_EQ_CHAR(xs, ys);

  const char *hello = "Hello";
  ASSERT_EQ_POINTER(hello, hello);
}
