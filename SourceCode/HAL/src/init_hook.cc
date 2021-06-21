#include "aarch64/boot-mmu.hh"
#include "hal/halt.hh"          // for never_return
#include "hal/init.hh"          // for init_board
#include "hal/init_hook.hh"     // for InitHookType, InitHook, InitHookPrio...
#include "hal/oops.hh"          // for oops
#include "hal/serial_port.hh"   // for is_serial_port_initialized
#include "libcxx/error_code.hh" // for KErrorCode, not_ok, KErrorCode::OK
#include "libcxx/log.hh"        // for log_f, log_d, log_e
#include "libcxx/types.hh"      // for uptr
#include "libcxx/utils.hh"      // for underlying_value

extern u64 __init_hooks_start;
extern u64 __init_hooks_end;

static KErrorCode run_init_hook(const InitHookType& init_hook_type) {
  // todo: 启用 mmu 后 __init_hooks_start,__init_hooks_end 是否会发生变化
  u64* init_hooks_start = kernel_relocated_base + &__init_hooks_start;
  u64* init_hooks_end   = kernel_relocated_base + &__init_hooks_end;
  auto init_hooks_count = (init_hooks_end - init_hooks_start) / (sizeof(InitHook) / sizeof(uptr));
  if (init_hooks_count == 0) { return KErrorCode::OK; }

  InitHook* init_hook = reinterpret_cast<InitHook*>(init_hooks_start);
  for (auto priority = underlying_value(InitHookPriority::HIGHEST);
       priority >= underlying_value(InitHookPriority::LOWEST); --priority) {
    for (decltype(init_hooks_count) i = 0; i < init_hooks_count; ++i) {
      const InitHook& hook = init_hook[i];
      if (hook.init_hook_type == init_hook_type) {
        if (underlying_value(hook.init_hook_priority) == priority) {
          auto result = hook.init_hook_function();
          if (not_ok(result)) { return result; }
        }
      }
    }
  }

  return KErrorCode::OK;
}

static KErrorCode pre_init_board() { return run_init_hook(InitHookType::PRE_INIT_BOARD); }
static KErrorCode post_init_board() { return run_init_hook(InitHookType::POST_INIT_BOARD); }

static KErrorCode pre_init_cpu() { return run_init_hook(InitHookType::PRE_INIT_CPU); }
static KErrorCode post_init_cpu() { return run_init_hook(InitHookType::POST_INIT_CPU); }

static KErrorCode pre_init_mmu() { return run_init_hook(InitHookType::PRE_INIT_MMU); };
static KErrorCode post_init_mmu() { return run_init_hook(InitHookType::POST_INIT_MMU); };

KErrorCode pre_kernel_main() { return run_init_hook(InitHookType::PRE_KERNEL_MAIN); }

[[noreturn]] KErrorCode post_kernel_main() {
  KErrorCode result = run_init_hook(InitHookType::POST_KERNEL_MAIN);
  if (not_ok(result)) {
    log_f("post kernel main hook failed");
    oops();
  }
  never_return("[DEBUG init.cc:76(post_kernel_main)] post kernel main, will never return\n");
}

KErrorCode init_board_with_hooks() {
  KErrorCode result = pre_init_board();
  if (not_ok((result))) {
    if (is_serial_port_initialized()) { log_e("pre init board failed, code: %d", underlying_value(result)); };
    return result;
  }

  result = init_board();
  if (not_ok(result)) {
    if (is_serial_port_initialized()) { log_f("init board failed"); }
    return result;
  }

  result = post_init_board();
  if (not_ok(result)) {
    log_f("post init board failed, code: %d", underlying_value(result));
    return result;
  }

  return result;
}
