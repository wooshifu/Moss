#pragma once

#include "libcxx/types.hpp"

extern u64 __text_start;
extern u64 __text_end;
extern u64 __rodata_start;
extern u64 __rodata_end;
extern u64 __data_start;
extern u64 __data_end;
extern u64 __bss_start;
extern u64 __bss_end;

inline u64 get_text_start_address() { return reinterpret_cast<u64>(&__text_start); }
inline u64 get_text_end_address() { return reinterpret_cast<u64>(&__text_end); }
inline u64 get_text_size() { return get_text_end_address() - get_text_start_address(); }

inline u64 get_bss_start_address() { return reinterpret_cast<u64>(&__bss_start); }
inline u64 get_bss_end_address() { return reinterpret_cast<u64>(&__bss_end); }
inline u64 get_bss_size() { return get_bss_end_address() - get_bss_start_address(); }
