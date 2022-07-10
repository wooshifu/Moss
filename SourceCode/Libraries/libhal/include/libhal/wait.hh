#pragma once

#include "libcxx/attr.hh"

attr_noreturn void arch_wait_infinitely();

attr_noreturn inline void wait_infinitely() { arch_wait_infinitely(); }
