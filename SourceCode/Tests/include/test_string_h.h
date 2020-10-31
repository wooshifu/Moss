#pragma once

#include "libc/string.h"

TEST(should_mem_clean_works) {
  i32 array[5] = {0x12345678, 0x9abcdef1, 0x23456789, 0xabcdef12, 0x3456789a};
  memzero((u8 *)array, (const u8 *)(array + 5));
  for (size_t index = 0; index < sizeof(i32) * 5; ++index) {
    ASSERT_EQ_UINT8(*((u8 *)(array) + index), 0);
  }
}

TEST(should_memset_set_memory) {
  i32 array[] = {1, 2, 3};
  size_t count = sizeof(array) / sizeof(array[0]);
  i32 *destination = memset(array, 0, count);
  for (size_t index = 0; index < count; ++index) {
    ASSERT_EQ_INT32(array[index], 0);
    ASSERT_EQ_INT32(destination[index], 0);
  }
}

TEST(should_strlen_return_correct_len) {
  const char *n = NULL;
  ASSERT_EQ_SIZE_T(strlen(n), 0);
  const char *a = "HelloWorld";
  ASSERT_EQ_SIZE_T(strlen(a), 10);
  const char *b = "HelloWorld\n";
  ASSERT_EQ_SIZE_T(strlen(b), 11);
  const char *c = "HelloWorld\n\r";
  ASSERT_EQ_SIZE_T(strlen(c), 12);
  const char *d = "HelloWorld\n\rAgain";
  ASSERT_EQ_SIZE_T(strlen(d), 17);
  const char *e = "HelloWorld\0";
  ASSERT_EQ_SIZE_T(strlen(e), 10);
}

TEST(should_strchr_works) {
  const char *msg = "ABCDabcd";
  char *A = strchr(msg, 'A');
  ASSERT_EQ_CHAR(*A, 'A');
  char *B = strchr(msg, 'B');
  ASSERT_EQ_CHAR(*B, 'B');
  char *a = strchr(msg, 'a');
  ASSERT_EQ_CHAR(*a, 'a');
  char *b = strchr(msg, 'b');
  ASSERT_EQ_CHAR(*b, 'b');
  char *non = strchr(msg, 'z');
  ASSERT_EQ_POINTER(non, NULL);
}

TEST(should_strrchr_works) {
  const char *msg = "ABCDabcd";
  char *A = strrchr(msg, 'A');
  ASSERT_EQ_CHAR(*A, 'A');
  char *B = strrchr(msg, 'B');
  ASSERT_EQ_CHAR(*B, 'B');
  char *a = strrchr(msg, 'a');
  ASSERT_EQ_CHAR(*a, 'a');
  char *b = strrchr(msg, 'b');
  ASSERT_EQ_CHAR(*b, 'b');
  char *non = strrchr(msg, 'z');
  ASSERT_EQ_POINTER(non, NULL);
}
