#pragma once

#include "libcxx/macro.hh"

/**
 * halt the cpu
 */
extern_C void halt();

/**
 * infinite loop, this function will never return
 */
extern_C [[noreturn]] void never_return();
