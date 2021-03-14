#pragma once

#include "hal/interface.hh"
#include "libcxx/macro.hh"

/**
 * halt the cpu
 */
HAL_INTERFACE extern_C void halt();

/**
 * infinite loop, this function will never return
 */
HAL_INTERFACE extern_C [[noreturn]] void never_return(const char* reason);

[[noreturn]] inline void never_return() { never_return("wfi, will never return\n"); }
