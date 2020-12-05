#include "aarch64/linker.hpp"
#include "libcxx/log.hpp"
#include "libcxx/string.hpp"

extern_C void bss_clean() {
  u64 bss_start = get_bss_start_address();
  u64 bss_end   = get_bss_end_address();
  u64 bss_size  = get_bss_size();
  if (likely(bss_start % 8 == 0 and bss_end % 8 == 0)) {
    log_d("fast bss clean");
    for (u64 index = 0; index < bss_size; ++index) {
      reinterpret_cast<u64 *>(bss_start)[index] = 0;
    }
    return;
  }

  memzero((u8 *)get_bss_start_address(), (u8 *)get_bss_end_address());
}
