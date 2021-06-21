#pragma once

#include "kernel/compiler.hh"
#include "libcxx/macro.hh"
#include "libcxx/types.hh"

// simple boot time allocator, used to carve off memory before the VM is completely up and running
extern_C {
  void boot_alloc_init();
  paddr_t boot_alloc_page_phys();
}
