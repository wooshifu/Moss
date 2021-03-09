#include <utility>       // for forward

#include "aarch64/linker.hh"
#include "array"         // for array
#include "kconfig.hh"    // for CONFIG_KERNEL_PRINT_MEMORY_LAYOUT
#include "libcxx/log.hh" // for log_d
#include "tuple"         // for make_tuple, get

void print_memory_layout() {
#if CONFIG_KERNEL_PRINT_MEMORY_LAYOUT
  using std::make_tuple;
  const std::array sections = {
      make_tuple("text", get_text_start_address(), get_text_end_address(), get_text_size()),
      make_tuple("rodata", get_rodata_start_address(), get_rodata_end_address(), get_rodata_size()),
      make_tuple("data", get_data_start_address(), get_data_end_address(), get_data_size()),
      make_tuple("init_entry", get_init_entry_start_address(), get_init_entry_end_address(), get_init_entry_size()),
      make_tuple("bss", get_bss_start_address(), get_bss_end_address(), get_bss_size()),
      make_tuple("symtab", get_symtab_start_address(), get_symtab_end_address(), get_symtab_size()),
      make_tuple("strtab", get_strtab_start_address(), get_strtab_end_address(), get_strtab_size()),
      make_tuple("interp", get_interp_start_address(), get_interp_end_address(), get_interp_size()),
      make_tuple("build_id", get_build_id_start_address(), get_build_id_end_address(), get_build_id_size()),
      make_tuple("stack", get_stack_start_address(), get_stack_end_address(), get_stack_size()),
  };

  constexpr const char* cutter_line{"---------------------------------------------------------------------------"};
  log_d("%s", cutter_line);
  using std::get;
  for (const auto& section : sections) {
    log_d("%15s: %016lX - %016lX, size: %lu bytes", get<0>(section), get<1>(section), get<2>(section), get<3>(section));
  }
  log_d("%s", cutter_line);
#endif
}
