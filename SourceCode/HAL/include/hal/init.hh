#pragma once

#include "hal/interface.hh"
#include "libcxx/error_code.hh"

HAL_INTERFACE KErrorCode init_board();

HAL_INTERFACE KErrorCode init_cpu();

HAL_INTERFACE KErrorCode init_mmu();
