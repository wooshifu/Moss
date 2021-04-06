#pragma once

#include "libcxx/types.hh"

extern u64 __text_start[];
extern u64 __text_end[];

extern u64 __init_hooks_start[];
extern u64 __init_hooks_end[];

extern u64 __rodata_start[];
extern u64 __rodata_end[];

extern u64 __data_start[];
extern u64 __data_end[];

extern u64 __bss_start[];
extern u64 __bss_end[];

extern u64 __stack_start[];
extern u64 __stack_end[];

#ifndef __generate_memory_layout_functions
#define __generate_memory_layout_functions(section_name, section_name_start, section_name_end)                         \
  inline u64 get_##section_name##_start_address() { return reinterpret_cast<u64>(section_name_start); }                \
  inline u64 get_##section_name##_end_address() { return reinterpret_cast<u64>(section_name_end); }                    \
  inline u64 get_##section_name##_size() {                                                                             \
    return get_##section_name##_end_address() - get_##section_name##_start_address();                                  \
  }
#endif

__generate_memory_layout_functions(text, __text_start, __text_end);
__generate_memory_layout_functions(init_hook, __init_hooks_start, __init_hooks_end);
__generate_memory_layout_functions(rodata, __rodata_start, __rodata_end);
__generate_memory_layout_functions(data, __data_start, __data_end);
__generate_memory_layout_functions(bss, __bss_start, __bss_end);
__generate_memory_layout_functions(stack, __stack_start, __stack_end);

void print_memory_layout();
