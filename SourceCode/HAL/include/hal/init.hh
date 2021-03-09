#pragma once

#include "libcxx/error_code.hh"

/// return value convention, zero on success, non zero on failure

///// function pointer to init hook function
using init_function_pointer = int (*)();

int pre_init_board();
int init_board();
int post_init_board();

int pre_init_cpu();
int init_cpu();
int post_init_cpu();

int pre_init_mmu();
int init_mmu();
int post_init_mmu();

void pre_kernel_main();
KErrorCode init_board_with_hooks();
[[noreturn]] void post_kernel_main();
