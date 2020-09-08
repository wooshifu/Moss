#include "arch/asm.h"
#include "hal/el.h"

uint64_t get_current_el() { return ARM64_MRS(CurrentEL) >> 2; }
