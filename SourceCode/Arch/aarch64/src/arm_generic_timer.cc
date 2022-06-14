#include "aarch64/intrin.hh"
#include "libcxx/macros.hh"
#include "libcxx/types.hh"

namespace arch {

  // This is the C++ type that the assembly macro `sample_ticks` delivers.
  // Higher-level kernel code knows how to translate this into the Zircon
  // monotonic clock's zx_ticks_t.
  struct EarlyTicks {
    u64 cntpct_el0, cntvct_el0;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-statement-expression"
    [[gnu::always_inline]] static EarlyTicks Get() {
      return {
          __arm_rsr64("cntpct_el0"),
          __arm_rsr64("cntvct_el0"),
      };
    }
#pragma clang diagnostic pop

    [[gnu::always_inline]] static EarlyTicks Zero() { return {0, 0}; }
  };
} // namespace arch

extern_C { arch::EarlyTicks kernel_virtual_entry_ticks; }
