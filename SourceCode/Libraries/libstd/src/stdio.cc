#include "libstd/stdio.hh"

#include "libstd/printf.hh"
#include "libstd/stdarg.hh"

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
