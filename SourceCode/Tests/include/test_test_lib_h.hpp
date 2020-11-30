#pragma once

#include "tests/test_lib.hpp"

TEST(should_test_lib_test_eq_works) {
  int x = 1;
  int y = 1;
  ASSERT_EQ(x, y);

  i8 xi8 = 0x12;
  i8 yi8 = 0x12;
  ASSERT_EQ(xi8, yi8);
  u8 xu8 = 0x21;
  u8 yu8 = 0x21;
  ASSERT_EQ(xu8, yu8);

  i16 xi16 = 0x1234;
  i16 yi16 = 0x1234;
  ASSERT_EQ(xi16, yi16);
  u16 xu16 = 0x4321;
  u16 yu16 = 0x4321;
  ASSERT_EQ(xu16, yu16);

  i32 xi32 = 0x12345678;
  i32 yi32 = 0x12345678;
  ASSERT_EQ(xi32, yi32);
  u32 xu32 = 0x87654321;
  u32 yu32 = 0x87654321;
  ASSERT_EQ(xu32, yu32);

  i64 xi64 = 0x1234567812345678;
  i64 yi64 = 0x1234567812345678;
  ASSERT_EQ(xi64, yi64);
  u64 xu64 = 0x1234567812345678;
  u64 yu64 = 0x1234567812345678;
  ASSERT_EQ(xu64, yu64);

  char xc = 'A';
  char yc = 'A';
  ASSERT_EQ(xc, yc);

  size_t xs = 1;
  size_t ys = 1;
  ASSERT_EQ(xs, ys);

  const char *hello  = "Hello";
  const char *hello1 = hello;
  ASSERT_EQ(hello, hello1);
}
