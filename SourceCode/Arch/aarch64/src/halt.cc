//
// Created by shifu on 2021/10/17.
//

#include "libcxx/macros.hh"

extern_C [[noreturn]] [[gnu::naked]] void hal_never_return() {
  asm volatile(R"(
    wfi
    b hal_never_return
  )");
}
