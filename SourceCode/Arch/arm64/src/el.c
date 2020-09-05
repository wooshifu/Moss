#include "hal/el.h"

uint64_t get_current_el() {
  uint64_t current_el = 0;
  asm volatile("mrs %0, CurrentEL" : "=r"(current_el));
  return current_el >> 2;
}
