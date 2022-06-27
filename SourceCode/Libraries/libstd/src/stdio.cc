#include "libstd/stdio.hh" // for stdout, _fprintf_output_func, printf

#include "libcxx/attr.hh"   // for attr_printf
#include "libcxx/types.hh"  // for size_t
#include "libio/io.hh"      // for console_io, FILE, io_write
#include "libstd/printf.hh" // for printf_engine
#include "libstd/stdarg.hh" // for va_list, va_end, va_start

// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
FILE stdio_FILEs[3] = {
    {.io = &console_io}, // stdin
    {.io = &console_io}, // stdout
    {.io = &console_io}, // stderr
};

int _fprintf_output_func(const char* str, size_t len, void* state) {
  FILE* fp = reinterpret_cast<FILE*>(state);

  return static_cast<int>(io_write(fp->io, str, len));
}

int vfprintf(FILE* fp, const char* fmt, va_list ap) { return printf_engine(&_fprintf_output_func, (void*)fp, fmt, ap); }

int fprintf(FILE* fp, const char* fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  int err = vfprintf(fp, fmt, ap);
  va_end(ap);
  return err;
}

attr_printf(1, 2) int printf(const char* fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  int err = vfprintf(stdout, fmt, ap);
  va_end(ap);

  return err;
}

int vprintf(const char* fmt, va_list ap) { return vfprintf(stdout, fmt, ap); }
