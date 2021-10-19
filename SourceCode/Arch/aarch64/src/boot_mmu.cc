//
// Created by shifu on 2021/10/17.
//

#include "libcxx/macros.hh"
#include "libcxx/types.hh"

// Early boot time page table creation code, called from start.S while running in physical address
// space with the mmu disabled. This code should be position independent as long as it sticks to basic code.
// todo: remove #pragma
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
[[gnu::used]] extern_C int arm64_boot_map(pte_t* kernel_table0, const vaddr_t vaddr, const paddr_t paddr,
                                          const size_t len, const pte_t flags) {
  // todo: not implemented
  return 0;
}
#pragma clang diagnostic pop

[[gnu::used]] extern_C void on_boot_map_failed() {}
