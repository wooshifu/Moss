#include "hal/halt.hh"          // for never_return
#include "hal/init.hh"          // for post_init_board, post_init_cpu, post_init...
#include "hal/oops.hh"          // for oops
#include "hal/serial_port.hh"   // for is_serial_port_initialized
#include "libcxx/error_code.hh" // for okj
#include "libcxx/log.hh"        // for log_d
#include "libcxx/macro.hh"      // for extern_C

static int run_init_hooks(const uptr* init_hook_start_address, const uptr* init_hook_end_address) {
  uptr* start_address                 = (uptr*)(init_hook_start_address);
  int result                          = 0;
  init_function_pointer init_function = nullptr;

  while (start_address < init_hook_end_address) {
    init_function = reinterpret_cast<init_function_pointer>(start_address);
    result        = init_function();
    if (not_ok(result)) {
      oops();
      break;
    }
    start_address += (1 << 3);
  }

  return result;
}

REGISTER_AS_POST_INIT_BOARD_HOOK static int post_init_board_function_test1() {
  if (is_serial_port_initialized()) {
    log_d("post_init_board hook 1");
  }
  return 0;
}

REGISTER_AS_POST_INIT_BOARD_HOOK static int post_init_board_function_test2() {
  if (is_serial_port_initialized()) {
    log_d("post_init_board hook 2");
  }
  return 0;
}

extern uptr __pre_init_board_hook_start;
extern uptr __pre_init_board_hook_end;
extern uptr __post_init_board_hook_start;
extern uptr __post_init_board_hook_end;

extern uptr __pre_init_cpu_hook_start;
extern uptr __pre_init_cpu_hook_end;
extern uptr __post_init_cpu_hook_start;
extern uptr __post_init_cpu_hook_end;

extern uptr __pre_init_mmu_hook_start;
extern uptr __pre_init_mmu_hook_end;
extern uptr __post_init_mmu_hook_start;
extern uptr __post_init_mmu_hook_end;

extern uptr __pre_kernel_main_hook_start;
extern uptr __pre_kernel_main_hook_end;
extern uptr __post_kernel_main_hook_start;
extern uptr __post_kernel_main_hook_end;

static int pre_init_board() { return run_init_hooks(&__pre_init_board_hook_start, &__pre_init_board_hook_end); }
static int post_init_board() { return run_init_hooks(&__post_init_board_hook_start, &__post_init_board_hook_end); }

static int pre_init_cpu() { return run_init_hooks(&__pre_init_cpu_hook_start, &__pre_init_cpu_hook_end); }
static int post_init_cpu() { return run_init_hooks(&__post_init_cpu_hook_start, &__post_init_cpu_hook_end); }

static int pre_init_mmu() { return run_init_hooks(&__pre_init_mmu_hook_start, &__pre_init_mmu_hook_end); };
static int post_init_mmu() { return run_init_hooks(&__post_init_mmu_hook_start, &__post_init_mmu_hook_end); };

int pre_kernel_main() { return run_init_hooks(&__pre_kernel_main_hook_start, &__pre_kernel_main_hook_end); }
[[noreturn]] void post_kernel_main() {
  int result = run_init_hooks(&__post_kernel_main_hook_start, &__post_kernel_main_hook_end);
  if (not_ok(result)) {
    log_f("post kernel main hook failed");
    oops();
  }
  never_return();
}

KErrorCode init_board_with_hooks() {
  auto result = pre_init_board();
  if (ok(result) and is_serial_port_initialized()) {
    log_f("pre init board failed, code: %d", result);
  }

  result = init_board();
  if (not_ok(result)) {
    if (!is_serial_port_initialized()) {
      return KErrorCode::UNKNOWN;
    }

    log_f("init board failed");
    oops();
  }

  result = post_init_board();
  if (not_ok(result)) {
    log_f("post init board failed, code: %d", result);
  }
  return KErrorCode::OK;
}
