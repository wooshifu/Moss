#include "libcxx/log.hpp"
#include "libcxx/stdlib.hpp"

#if 0
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
#endif
