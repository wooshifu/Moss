#include "array"                // for array
#include "hal/halt.hh"          // for never_return
#include "hal/init.hh"          // for post_init_board, post_init_cpu, post_init...
#include "hal/oops.hh"          // for oops
#include "hal/serial_port.hh"   // for is_serial_port_initialized
#include "libcxx/error_code.hh" // for okj
#include "libcxx/log.hh"        // for log_d
#include "libcxx/macro.hh"      // for extern_C

static int init_function_hook(auto init_functions) {
  for (const auto& function : init_functions) {
    auto result = function();
    if (not_ok(result)) {
      return result;
    }
    return result;
  }
  return 0;
}

static std::array pre_init_board_functions = {[]() { return 0; }};
static std::array post_init_board_functions{
    []() {
      if (is_serial_port_initialized()) {
        log_d("post_init_board hook");
      }
      return 0;
    },
};
int pre_init_board() { return init_function_hook(pre_init_board_functions); }
int post_init_board() { return init_function_hook(post_init_board_functions); }

static std::array pre_init_cpu_functions{
    []() {
      log_d("pre_init_cpu hook");
      return 0;
    },
};
static std::array post_init_cpu_functions{
    []() {
      log_d("post_init_cpu hook");
      return 0;
    },
};
int pre_init_cpu() { return init_function_hook(pre_init_cpu_functions); }
int post_init_cpu() { return init_function_hook(post_init_cpu_functions); }

static std::array pre_init_mmu_functions{
    []() {
      log_d("pre_init_mmu hook");
      return 0;
    },
};
static std::array post_init_mmu_functions{
    []() {
      log_d("post_init_mmu hook");
      return 0;
    },
};
int pre_init_mmu() { return init_function_hook(pre_init_mmu_functions); };
int post_init_mmu() { return init_function_hook(post_init_mmu_functions); };

// kernel_main hooks
extern_C int init_exception_vector_table();
static std::array before_kernel_main_functions = {
    init_exception_vector_table, /// exception vector table should be initialized first
};
void pre_kernel_main() {
  for (const auto& before_kernel_main_function : before_kernel_main_functions) {
    before_kernel_main_function();
  }
}

static std::array post_kernel_main_functions = {
    []() { log_d("post kernel main"); },
};
[[noreturn]] void post_kernel_main() {
  for (const auto& post_kernel_main_function : post_kernel_main_functions) {
    post_kernel_main_function();
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
