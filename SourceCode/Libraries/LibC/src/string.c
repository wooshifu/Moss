#include "libc/log.h"
#include "libc/string.h"

void memzero(u8 *mem_start, const u8 *mem_end) {
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

char *strcpy(char *destination, const char *source) { return strncpy(destination, source, strlen(source) + 1); }
char *strncpy(char *destination, const char *source, usize count) {
  if (count != 0) {
    char *d = destination;
    const char *s = source;

    do {
      if ((*d++ = *s++) == 0) {
        /* NUL pad the remaining count-1 bytes */
        while (--count != 0) {
          *d++ = 0;
        }
        break;
      }
    } while (--count != 0);
  }

  return (destination);
}

char *strcat(char *destination, const char *source) {
  char *tmp = destination;

  while (*destination) {
    destination++;
  }
  while ((*destination++ = *source++) != '\0') {
  }

  return tmp;
}
char *strncat(char *destination, const char *source, usize count) {
  char *tmp = destination;

  if (count) {
    while (*destination) {
      destination++;
    }
    while ((*destination++ = *source++)) {
      if (--count == 0) {
        *destination = '\0';
        break;
      }
    }
  }

  return tmp;
}
