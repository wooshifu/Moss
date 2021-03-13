#pragma once

#include "libcxx/types.hh"

inline void memory_write_8bits(u8* address, u8 value) { *address = value; }
inline void memory_write_16bits(u16* address, u16 value) { *address = value; }
inline void memory_write_32bits(u32* address, u32 value) { *address = value; }
inline void memory_write_64bits(u64* address, u64 value) { *address = value; }

inline u8 memory_read_8bits(const u8* address) { return *address; }
inline u16 memory_read_16bits(const u16* address) { return *address; }
inline u32 memory_read_32bits(const u32* address) { return *address; }
inline u64 memory_read_64bits(const u64* address) { return *address; }
