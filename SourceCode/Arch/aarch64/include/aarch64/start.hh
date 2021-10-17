//
// Created by shifu on 2021/10/14.
//

#pragma once

#include "libcxx/types.hh"

inline uint64_t get_kernel_entry_paddr() {
  extern uint64_t _kernel_entry_paddr;
  return _kernel_entry_paddr;
}

inline uint64_t get_bool_el() {
  extern uint64_t _arch_boot_el;
  return _arch_boot_el;
}
