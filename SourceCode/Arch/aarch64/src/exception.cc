#include "aarch64/exception.hh"

static bool exception_vector_table_already_initialized = false;

/// this is a init hook function, so detect it's initialized or not
extern_C void init_exception_vector_table() {
  [[likely]] if (not exception_vector_table_already_initialized) {
    init_exception_vector_table_asm();
    exception_vector_table_already_initialized = true;
  }
}
