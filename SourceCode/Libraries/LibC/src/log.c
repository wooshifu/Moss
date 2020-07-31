#include "log.h"

const char *__extract_file_name_from_path(const char *file_path) {
  char *file_name = strrchr(file_path, '/');
  if (file_name != NULL) {
    // todo: filename + 1 may cause expected behavior
    return file_name + 1;
  }

  file_name = strrchr(file_path, '\\');
  if (file_name != NULL) {
    // todo: filename + 1 may cause expected behavior
    return file_name + 1;
  }

  return file_path;
}

#ifndef __do_log
#define __do_log(format)                                                                                               \
  do {                                                                                                                 \
    va_list args;                                                                                                      \
    va_start(args, format);                                                                                            \
    vprintf(format, args);                                                                                             \
    va_end(args);                                                                                                      \
  } while (0)
#endif

void _log_v(const char *format, ...) {
#if LOG_LEVEL <= LOG_LEVEL_VERBOSE
  __do_log(format);
#endif
}

void _log_d(const char *format, ...) {
#if LOG_LEVEL <= LOG_LEVEL_DEBUG
  __do_log(format);
#endif
}

void _log_i(const char *format, ...) {
#if LOG_LEVEL <= LOG_LEVEL_INFO
  __do_log(format);
#endif
}

void _log_w(const char *format, ...) {
#if LOG_LEVEL <= LOG_LEVEL_WARNING
  __do_log(format);
#endif
}

void _log_e(const char *format, ...) {
#if LOG_LEVEL <= LOG_LEVEL_ERROR
  __do_log(format);
#endif
}

void _log_f(const char *format, ...) {
#if LOG_LEVEL <= LOG_LEVEL_FATAL
  __do_log(format);
#endif
}
