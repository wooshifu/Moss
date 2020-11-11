#include "libc/log.hpp"
#include "libc/stdlib.hpp"

void abort() {
  /// todo: not implemented
  log_d("aborted!!!");
}

void exit(int exit_code) {
  /// todo: not implemented
  log_d("exited, code: %d", exit_code);
}

int atexit(void (*func)()) {
  func();
  return 0;
}
