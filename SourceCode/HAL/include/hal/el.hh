#pragma once

#include "hal/interface.hh"
#include "libcxx/types.hh"

/**
 * get current exception level
 * @return current exception level
 */
HAL_INTERFACE u64 get_current_el();
