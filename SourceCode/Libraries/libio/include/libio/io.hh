#pragma once

#include "libcxx/list.hh"  // for list_node
#include "libcxx/types.hh" // for size_t, ssize_t, u32

struct print_callback {
  list_node entry;
  void (*print)(print_callback* cb, const char* str, size_t len);
  void* context;
};

struct io_handle_t;

struct io_handle_hooks_t {
  ssize_t (*write)(io_handle_t* handle, const char* buf, size_t len);
  ssize_t (*read)(io_handle_t* handle, char* buf, size_t len);
};

struct io_handle_t {
  u32 magic;
  const io_handle_hooks_t* hooks;
};

struct FILE {
  io_handle_t* io;
};

/* routines to call through the io handle */
ssize_t io_write(io_handle_t* io, const char* buf, size_t len);
ssize_t io_read(io_handle_t* io, char* buf, size_t len);

constexpr auto IO_HANDLE_MAGIC = 0x696f6820; // "ioh "

extern io_handle_t console_io;
