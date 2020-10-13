#include "arm64/asm.h"
#include "hal/el.h"

uint64_t get_current_el() { return ARM64_READ_SYSTEM_REGISTER(CurrentEL) >> 2; }
