#include "aarch64/linker.hh"
#include "libcxx/log.hh"

extern_C void print_mem_stack() {
#if 1
  log_d("__text_start is %p", &__text_start);
  log_d("__text_end is %p", &__text_end);
  log_d("__rodata_start is %p", &__rodata_start);
  log_d("__rodata_end is %p", &__rodata_end);
  log_d("__data_start is %p", &__data_start);
  log_d("__data_end is %p", &__data_end);
  log_d("__bss_start is %p", &__bss_start);
  log_d("__bss_end is %p", &__bss_end);
#endif
}
