#include "libstd/string.hh"

size_t strlen(char const* str) {
  size_t index = 0;

  while (str[index] != '\0') {
    index += 1;
  }

  return index;
}
