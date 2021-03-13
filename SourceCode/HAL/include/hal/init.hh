#pragma once

#include "libcxx/error_code.hh"

/// return value convention, zero on success, non zero on failure

///// function pointer to init hook function
using init_function_pointer = int (*)();

int init_board();

int init_cpu();

int init_mmu();

int pre_kernel_main();
KErrorCode init_board_with_hooks();
[[noreturn]] void post_kernel_main();

#ifndef SECTION_PRE_INIT_BOARD_HOOK
#define SECTION_PRE_INIT_BOARD_HOOK section(".pre_init_board_hook")
#endif
#ifndef SECTION_POST_INIT_BOARD_HOOK
#define SECTION_POST_INIT_BOARD_HOOK section(".post_init_board_hook")
#endif

#ifndef SECTION_PRE_INIT_CPU_HOOK
#define SECTION_PRE_INIT_CPU_HOOK section(".pre_init_cpu_hook")
#endif
#ifndef SECTION_POST_INIT_CPU_HOOK
#define SECTION_POST_INIT_CPU_HOOK section(".post_init_cpu_hook")
#endif

#ifndef SECTION_PRE_INIT_MMU_HOOK
#define SECTION_PRE_INIT_MMU_HOOK section(".pre_init_mmu_hook")
#endif
#ifndef SECTION_POST_INIT_MMU_HOOK
#define SECTION_POST_INIT_MMU_HOOK section(".post_init_mmu_hook")
#endif

#ifndef SECTION_PRE_KERNEL_MAIN_HOOK
#define SECTION_PRE_KERNEL_MAIN_HOOK section(".pre_kernel_main_hook")
#endif
#ifndef SECTION_POST_KERNEL_MAIN_HOOK
#define SECTION_POST_KERNEL_MAIN_HOOK section(".post_kernel_main_hook")
#endif

// todo: how to customize init function priority?
#ifndef REGISTER_AS_PRE_INIT_BOARD_HOOK
#define REGISTER_AS_PRE_INIT_BOARD_HOOK [[gnu::SECTION_PRE_INIT_BOARD_HOOK]]
#endif

#ifndef REGISTER_AS_POST_INIT_BOARD_HOOK
#define REGISTER_AS_POST_INIT_BOARD_HOOK [[gnu::SECTION_POST_INIT_BOARD_HOOK]]
#endif

#ifndef REGISTER_AS_PRE_INIT_CPU_HOOK
#define REGISTER_AS_PRE_INIT_CPU_HOOK [[gnu::SECTION_PRE_INIT_CPU_HOOK]]
#endif

#ifndef REGISTER_AS_POST_INIT_CPU_HOOK
#define REGISTER_AS_POST_INIT_CPU_HOOK [[gnu::SECTION_POST_INIT_CPU_HOOK]]
#endif

#ifndef REGISTER_AS_PRE_INIT_MMU_HOOK
#define REGISTER_AS_PRE_INIT_MMU_HOOK [[gnu::SECTION_PRE_INIT_MMU_HOOK]]
#endif

#ifndef REGISTER_AS_POST_INIT_MMU_HOOK
#define REGISTER_AS_POST_INIT_MMU_HOOK [[gnu::SECTION_POST_INIT_MMU_HOOK]]
#endif

#ifndef REGISTER_AS_PRE_KERNEL_MAIN_HOOK
#define REGISTER_AS_PRE_KERNEL_MAIN_HOOK [[gnu::SECTION_PRE_KERNEL_MAIN_HOOK]]
#endif

#ifndef REGISTER_AS_POST_KERNEL_MAIN_HOOK
#define REGISTER_AS_POST_KERNEL_MAIN_HOOK [[gnu::SECTION_POST_KERNEL_MAIN_HOOK]]
#endif
