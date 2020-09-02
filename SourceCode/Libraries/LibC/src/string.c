#include "libc/log.h"
#include "libc/string.h"

void memzero(uint8_t *mem_start, const uint8_t *mem_end) {
  log_d("mem start is: %p, mem end is: %p", mem_start, mem_end);
  if (mem_start > mem_end) {
    log_e("mem start: %p greater than mem end: %p, mem not cleaned", mem_start, mem_end);
    return;
  }
  for (size_t i = 0; mem_start + i < mem_end; i++) {
    *(mem_start + i) = '\0';
  }
}

void *memset(void *destination, int character, size_t count) {
  for (size_t index = 0; index < count; ++index) {
    ((int *)destination)[index] = character;
  }
  return destination;
}

size_t strlen(const char *str) {
  if (str == NULL) {
    return 0;
  }

  size_t len = 0;
  while (*str++) {
    ++len;
  }
  return len;
}

int strcmp(const char *left, const char *right) {
  // todo: not implemented
  return 0;
}

char *strchr(const char *str, int character) {
  if (str == NULL) {
    return NULL;
  }

  while (*str) {
    if (*str == character) {
      return (char *)str;
    }
    ++str;
  }
  return NULL;
}

char *strrchr(const char *str, int character) {
  if (str == NULL) {
    return NULL;
  }

  // todo: optimization. two loops here now
  size_t len = strlen(str);
  for (size_t index = len; index > 0; --index) {
    const char *string = str + index - 1;
    if (*string == character) {
      return (char *)string;
    }
  }
  return NULL;
}
