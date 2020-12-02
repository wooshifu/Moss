#include "aarch64/asm.hpp"
#include "hal/el.hpp"

u64 get_current_el() { return ARM64_READ_SYSREG(CurrentEL) >> 2; }
