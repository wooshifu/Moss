#pragma once

#include "libcxx/types.hh"
#include "rpi3/rpi3.hh"

namespace NS_rpi3 {
  void handle_local_timer_irq();
  void init_generic_timer();
  void handle_generic_timer_irq();
} // namespace NS_rpi3
