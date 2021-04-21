#include "hal/el.hh"

#include "aarch64/sysreg.hh"
#include "libcxx/types.hh"

u64 get_current_el() { return ARM64_READ_SYSREG(CurrentEL) >> 2; }
