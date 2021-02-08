#include "libcxx/linux_string.hh" // for memset, memzero, strcat, strchr
#include "libcxx/log.hh"          // for log_d, log_e
#include "libcxx/types.hh"        // for size_t, u8

void memzero(u8* mem_start, const u8* mem_end) {
  log_d("mem start is: %p, mem end is: %p", mem_start, mem_end);
  if (mem_start > mem_end) {
    log_e("mem start: %p greater than mem end: %p, mem not cleaned", mem_start, mem_end);
    return;
  }
  for (size_t i = 0; mem_start + i < mem_end; i++) {
    *(mem_start + i) = 0;
  }
}

void* memset(void* destination, int character, size_t count) {
  for (size_t index = 0; index < count; ++index) {
    (reinterpret_cast<u8*>(destination))[index] = character;
  }
  return destination;
}

size_t strlen(const char* str) {
  if (str == nullptr) {
    return 0;
  }

  size_t len = 0;
  while (*str++) {
    ++len;
  }
  return len;
}

char* strchr(const char* str, int character) {
  if (str == nullptr) {
    return nullptr;
  }

  while (*str) {
    if (*str == character) {
      return (char*)str;
    }
    ++str;
  }
  return nullptr;
}

char* strrchr(const char* str, int character) {
  if (str == nullptr) {
    return nullptr;
  }

  // todo: optimization. two loops here now
  size_t len = strlen(str);
  for (size_t index = len; index > 0; --index) {
    const char* string = str + index - 1;
    if (*string == character) {
      return (char*)string;
    }
  }
  return nullptr;
}

char* strcpy(char* destination, const char* source) { return strncpy(destination, source, strlen(source) + 1); }
char* strncpy(char* destination, const char* source, size_t count) {
  if (count != 0) {
    char* d       = destination;
    const char* s = source;

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

char* strcat(char* destination, const char* source) {
  char* tmp = destination;

  while (*destination) {
    destination++;
  }
  while ((*destination++ = *source++) != '\0') {
  }

  return tmp;
}
char* strncat(char* destination, const char* source, size_t count) {
  char* tmp = destination;

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

int strcmp(const char* left, const char* right) {
  while (true) {
    auto c1 = *left++;
    auto c2 = *right++;
    if (c1 != c2) {
      return c1 < c2 ? -1 : 1;
    }
    if (!c1) {
      break;
    }
  }
  return 0;
}
