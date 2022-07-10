#include "libcxx/attr.hh"  // for attr_unused
#include "libcxx/types.hh" // for size_t, ssize_t
#include "libhal/io.hh"    // for platform_dputc
#include "libio/io.hh"     // for io_handle_t, IO_HANDLE_MAGIC, io_handle_h...

/* print lock must be held when invoking out, outs, outc */
static void out_count(const char* str, size_t len) {
  /* write out the serial port */
  for (size_t i = 0; i < len; i++) {
    platform_dputc(str[i]);
  }
}

static ssize_t __debug_stdio_write(attr_unused io_handle_t* io, const char* str, size_t len) {
  out_count(str, len);
  return static_cast<ssize_t>(len);
}

static ssize_t __debug_stdio_read(io_handle_t* io, char* str, size_t len) { // NOLINT(readability-non-const-parameter)
  // todo: not implemented
  (void)(io);
  (void)(str);
  (void)(len);
  return 0;
}

/* global console io handle */
static const io_handle_hooks_t console_io_hooks = {
    .write = __debug_stdio_write,
    .read  = __debug_stdio_read,
};

io_handle_t console_io = {.magic = (IO_HANDLE_MAGIC), .hooks = &console_io_hooks};
