#pragma once

#include "libcxx/log.hh"

static inline void print_board_name() {
#ifdef BOARD_NAME
  log_i("board name: " BOARD_NAME);
#else
  log_i("board name: UNKNOWN");
#endif
}
