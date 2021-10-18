//
// Created by shifu on 2021/10/17.
//

#include "kconfig.hh"
#include "libcxx/macros.hh"
#include "libcxx/types.hh"

#if CONFIG_KERNEL_DISABLE_KASLR
[[gnu::used]] u64 kernel_relocated_base = 0xffff'ffff'0000'0000;
#else
#error "KASLR currently not implementated"
#endif

extern_C [[gnu::visibility("hidden")]] const char __kernel_end[]; // End of the image, including ".bss"

// store the start and current pointer to the boot allocator in physical address
u64 boot_alloc_start;
u64 boot_alloc_end;
// run in physical space without the mmu set up, so by computing the address of __kernel_end
// and saving it, we've effectively computed the physical address of the end of the
// kernel.
// We can't allow asan to check the globals here as it happens on a different
// aspace where asan shadow isn't mapped.
// todo: what's this???
/*[[gnu::visibility("hidden")]]*/
[[gnu::used]] extern "C" /*NO_ASAN*/ /*__NO_SAFESTACK*/ void boot_alloc_init() {
  boot_alloc_start = reinterpret_cast<u64>(__kernel_end);
  // TODO(fxbug.dev/32414): This is a compile-time no-op that defeats any compiler
  // optimizations based on its knowledge/assumption that `&__kernel_end` is a
  // constant here that equals the `&__kernel_end` constant as computed elsewhere.
  // Without this, the compiler can see that boot_alloc_start is never set to
  // any other value and replace code that uses the boot_alloc_start value
  // with code that computes `&__kernel_end` on the spot.  What the compiler doesn't
  // know is that this `&__kernel_end` is crucially a PC-relative computation when
  // the PC is a (low) physical address.  Later code that uses
  // boot_alloc_start will be running at a kernel (high) virtual address and
  // so its `&__kernel_end` will be nowhere near the same value.  The compiler isn't
  // wrong to do this sort of optimization when it can and other such cases
  // will eventually arise.  So long-term we'll need more thorough
  // compile-time separation of the early boot code that runs in physical
  // space from normal kernel code.  For now, this asm generates no
  // additional code but tells the compiler that it has no idea what value
  // boot_alloc_start might take, so it has to compute the `&__kernel_end` value now.
  __asm__("" : "=g"(boot_alloc_start) : "0"(boot_alloc_start));
  boot_alloc_end = reinterpret_cast<u64>(__kernel_end);
}
