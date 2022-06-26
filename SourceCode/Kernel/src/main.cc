#include "libcxx/attr.hh"
#include "libcxx/macros.hh"
#include "libcxx/types.hh"
#include "libstd/stdio.hh"

static void print_test() {
  int xx = 0;
  // todo: %.2f not working
  printf("welcome to moss, %s. %d, %08d, 0x%X, %f, %.2f, %c, %p\n", "hello world!", 123, 456, 0x123ab456, 0.123, 13.24,
         'A', (void*)(&xx));
}

extern_C attr_noreturn attr_used void kernel_main(attr_maybe_unused paddr_t handoff_paddr) {
  print_test();

  while (true) {
    asm volatile("wfe");
  }
}
