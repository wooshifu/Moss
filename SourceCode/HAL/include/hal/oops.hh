#pragma once

#include "hal/interface.hh"
#include "libcxx/macro.hh"

HAL_INTERFACE extern_C [[noreturn]] void oops();
[[noreturn]] void oops(const char* reason);
