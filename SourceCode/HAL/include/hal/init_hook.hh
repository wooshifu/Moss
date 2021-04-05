#pragma once

#include "libcxx/error_code.hh"
#include "libcxx/types.hh"

enum class InitHookType : uint32_t {
  PRE_KERNEL_MAIN,
  PRE_INIT_BOARD,
  POST_INIT_BOARD,
  PRE_INIT_CPU,
  POST_INIT_CPU,
  PRE_INIT_MMU,
  POST_INIT_MMU,
  POST_KERNEL_MAIN,
};

// currently, I think 10 is enough
enum class InitHookPriority : int32_t {
  LOWEST = 0,
  _1,
  _2,
  _3,
  _4,
  _5,
  _6,
  _7,
  _8,
  HIGHEST,
};

using InitHookFunction = KErrorCode (*)();

struct InitHook {
  InitHookType init_hook_type;
  InitHookPriority init_hook_priority;
  //  InitHookFlags init_hook_flags; // todo: add flags
  InitHookFunction init_hook_function;
  const char* init_hook_name;
};

#ifdef INIT_HOOK_SECTION
#error "macro INIT_HOOK_SECTION should not be override"
#endif
#define INIT_HOOK_SECTION alignas(struct InitHook) [[gnu::used, gnu::section(".init_hook")]]

#ifdef AS_INIT_HOOK_FLAGS
#error "macro AS_INIT_HOOK_FLAGS should not be override"
#endif
#define AS_INIT_HOOK_FLAGS(name, hook_function, type, /*flags,*/ priority)                                             \
  static const InitHook _init_hook_##name INIT_HOOK_SECTION = {                                                        \
      .init_hook_type     = type,                                                                                      \
      .init_hook_priority = priority, /* .init_hook_flags    = flags,// todo: add flags */                             \
      .init_hook_function = hook_function,                                                                             \
      .init_hook_name     = #name,                                                                                     \
  };

#ifdef AS_INIT_HOOK
#error "macro AS_INIT_HOOK should not be override"
#endif
#define AS_INIT_HOOK(hook_function, type, priority)                                                                    \
  AS_INIT_HOOK_FLAGS(hook_function, hook_function, type, /*InitHookFlags::NO_FLAG,*/ priority)

#ifdef AS_INIT_HOOK_PRE_KERNEL_MAIN
#error "macro AS_INIT_HOOK_PRE_KERNEL_MAIN should not be override"
#endif
#define AS_INIT_HOOK_PRE_KERNEL_MAIN(hook_function, priority)                                                          \
  AS_INIT_HOOK(hook_function, InitHookType::PRE_KERNEL_MAIN, priority)

#ifdef AS_INIT_HOOK_PRE_INIT_BOARD
#error "macro AS_INIT_HOOK_PRE_INIT_BOARD should not be override"
#endif
#define AS_INIT_HOOK_PRE_INIT_BOARD(hook_function, priority)                                                           \
  AS_INIT_HOOK(hook_function, InitHookType::PRE_INIT_BOARD, priority)

#ifdef AS_INIT_HOOK_POST_INIT_BOARD
#error "macro AS_INIT_HOOK_POST_INIT_BOARD should not be override"
#endif
#define AS_INIT_HOOK_POST_INIT_BOARD(hook_function, priority)                                                          \
  AS_INIT_HOOK(hook_function, InitHookType::POST_INIT_BOARD, priority)

#ifdef AS_INIT_HOOK_PRE_INIT_CPU
#error "macro AS_INIT_HOOK_PRE_INIT_CPU should not be override"
#endif
#define AS_INIT_HOOK_PRE_INIT_CPU(hook_function, priority)                                                             \
  AS_INIT_HOOK(hook_function, InitHookType::PRE_INIT_CPU, priority)

#ifdef AS_INIT_HOOK_POST_INIT_CPU
#error "macro AS_INIT_HOOK_POST_INIT_CPU should not be override"
#endif
#define AS_INIT_HOOK_POST_INIT_CPU(hook_function, priority)                                                            \
  AS_INIT_HOOK(hook_function, InitHookType::POST_INIT_CPU, priority)

#ifdef AS_INIT_HOOK_POST_KERNEL_MAIN
#error "macro AS_INIT_HOOK_POST_KERNEL_MAIN should not be override"
#endif
#define AS_INIT_HOOK_POST_KERNEL_MAIN(hook_function, priority)                                                         \
  AS_INIT_HOOK(hook_function, InitHookType::POST_KERNEL_MAIN, priority)

KErrorCode pre_kernel_main();
KErrorCode init_board_with_hooks();
[[noreturn]] KErrorCode post_kernel_main();
