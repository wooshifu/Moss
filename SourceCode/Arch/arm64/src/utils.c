#include "libc/log.h"
#include "libc/string.h"

extern u8 __text_start;
extern u8 __text_end;
extern u8 __rodata_start;
extern u8 __rodata_end;
extern u8 __data_start;
extern u8 __data_end;
extern u8 __bss_start;
extern u8 __bss_end;
extern u8 __svc_stack;
extern u8 __irq_stack;
extern u8 __fiq_stack;
extern u8 __sys_stack;
extern u8 __end_stack;
extern u8 __PAGE_TABLE;
extern u8 __HEAP_BEGIN;

void print_mem_stack() {
#if 0
  log_d("__text_start is %p", &__text_start);
  log_d("__text_end is %p", &__text_end);
  log_d("__rodata_start is %p", &__rodata_start);
  log_d("__rodata_end is %p", &__rodata_end);
  log_d("__data_start is %p", &__data_start);
  log_d("__data_end is %p", &__data_end);
  log_d("__bss_start is %p", &__bss_start);
  log_d("__bss_end is %p", &__bss_end);
  log_d("__svc_stack is %p", &__svc_stack);
  log_d("__irq_stack is %p", &__irq_stack);
  log_d("__fiq_stack is %p", &__fiq_stack);
  log_d("__sys_stack is %p", &__sys_stack);
  log_d("__end_stack is %p", &__end_stack);
  log_d("__PAGE_TABLE is %p", &__PAGE_TABLE);
  log_d("__HEAP_BEGIN is %p", &__HEAP_BEGIN);
#endif
}

void bss_clean() { memzero(&__bss_start, &__bss_end); }
