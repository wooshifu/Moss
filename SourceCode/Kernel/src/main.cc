//
// Created by shifu on 2021/10/14.
//

#include "hal/never_return.hh"
#include "libcxx/macros.hh"

[[gnu::used]] [[noreturn]] extern_C void kernel_main() { hal_never_return(); }
