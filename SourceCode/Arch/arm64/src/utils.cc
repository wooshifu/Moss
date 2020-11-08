#include "libc/log.h"
#include "libc/string.h"

extern u64 __text_start;
extern u64 __text_end;
extern u64 __rodata_start;
extern u64 __rodata_end;
extern u64 __data_start;
extern u64 __data_end;
extern u64 __bss_start;
extern u64 __bss_end;

extern "C" void print_mem_stack() {
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

extern "C" void bss_clean() { memzero((u8*)&__bss_start, (u8*)&__bss_end); }
