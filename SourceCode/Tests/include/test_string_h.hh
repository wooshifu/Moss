#pragma once

#include "libcxx/string.hh"

TEST(should_mem_clean_works) {
  i32 array[5] = {0x12345678, 0x1abcdef1, 0x23456789, 0x1bcdef12, 0x3456789a};
  memzero((u8 *)array, (const u8 *)(array + 5));
  for (size_t index = 0; index < sizeof(i32) * 5; ++index) {
    ASSERT_EQ(*((u8 *)(array) + index), 0);
  }
}

TEST(should_memset_set_memory) {
  i8 array[]      = {1, 2, 3};
  size_t count    = sizeof(array) / sizeof(array[0]);
  i8 *destination = static_cast<i8 *>(memset(array, 0, count));
  for (size_t index = 0; index < count; ++index) {
    ASSERT_EQ(array[index], 0);
    ASSERT_EQ(destination[index], 0);
  }
}

TEST(should_strlen_return_correct_len) {
  const char *n = nullptr;
  ASSERT_EQ(strlen(n), 0);
  const char *a = "HelloWorld";
  ASSERT_EQ(strlen(a), 10);
  const char *b = "HelloWorld\n";
  ASSERT_EQ(strlen(b), 11);
  const char *c = "HelloWorld\n\r";
  ASSERT_EQ(strlen(c), 12);
  const char *d = "HelloWorld\n\rAgain";
  ASSERT_EQ(strlen(d), 17);
  const char *e = "HelloWorld\0";
  ASSERT_EQ(strlen(e), 10);
}

TEST(should_strchr_works) {
  const char *msg = "ABCDabcd";
  char *A         = strchr(msg, 'A');
  ASSERT_EQ(*A, 'A');
  char *B = strchr(msg, 'B');
  ASSERT_EQ(*B, 'B');
  char *a = strchr(msg, 'a');
  ASSERT_EQ(*a, 'a');
  char *b = strchr(msg, 'b');
  ASSERT_EQ(*b, 'b');
  char *non = strchr(msg, 'z');
  ASSERT_EQ(non, nullptr);
}

TEST(should_strrchr_works) {
  const char *msg = "ABCDabcd";
  char *A         = strrchr(msg, 'A');
  ASSERT_EQ(*A, 'A');
  char *B = strrchr(msg, 'B');
  ASSERT_EQ(*B, 'B');
  char *a = strrchr(msg, 'a');
  ASSERT_EQ(*a, 'a');
  char *b = strrchr(msg, 'b');
  ASSERT_EQ(*b, 'b');
  char *non = strrchr(msg, 'z');
  ASSERT_EQ(non, nullptr);
}
