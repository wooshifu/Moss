#include "arch/align.hh"
#include "arch/defines.hh"
#include "kernel/compiler.hh"
#include "libcxx/types.hh"

#if 0
#include "vm/bootalloc.hh"

#include "align.hh"
#include "lib/instrumentation/asan.hh"
#include "stdint.hh"
#include "stdlib.hh"
#include "sys/types.hh"
#include "trace.hh"

#include "vm/physmap.hh"
#include "vm/pmm.hh"
#include "vm/vm.hh"

#include "vm_priv.hh"

#define LOCAL_TRACE VM_GLOBAL_TRACE(0)

// Simple boot time allocator that starts by allocating physical memory off
// the end of wherever the kernel is loaded in physical space.
//
// Pointers are returned from the kernel's physmap
#endif

//using paddr_t = unsigned long;
// store the start and current pointer to the boot allocator in physical address
paddr_t boot_alloc_start;
paddr_t boot_alloc_end;

#if defined(__clang__)
#define __NO_ASAN [[clang::no_sanitize("address")]]
#else
#define __NO_ASAN __attribute__((no_sanitize_address))
#endif
#define NO_ASAN __NO_ASAN
extern "C" __LOCAL const char __kernel_end[];               // End of the image, including ".bss"

// run in physical space without the mmu set up, so by computing the address of __kernel_end
// and saving it, we've effectively computed the physical address of the end of the
// kernel.
// We can't allow asan to check the globals here as it happens on a different
// aspace where asan shadow isn't mapped.
extern "C" NO_ASAN __NO_SAFESTACK void boot_alloc_init() {
  boot_alloc_start = reinterpret_cast<paddr_t>(__kernel_end);
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
  boot_alloc_end = reinterpret_cast<paddr_t>(__kernel_end);
}

#if 0
void boot_alloc_reserve(paddr_t start, size_t len) {
  uintptr_t end = ALIGN((start + len), PAGE_SIZE);

  if (end >= boot_alloc_start) {
    if ((start > boot_alloc_start) && ((start - boot_alloc_start) > (128 * 1024 * 1024))) {
      // if we've got 128MB of space, that's good enough
      // it's possible that the start may be *way* far up
      // (gigabytes) and there may not be space after it...
      return;
    }
    boot_alloc_start = boot_alloc_end = end;
  }
}

void* boot_alloc_mem(size_t len) {
  uintptr_t ptr;

  ptr = ALIGN(boot_alloc_end, 8);
  boot_alloc_end = (ptr + ALIGN(len, 8));

  LTRACEF("len %zu, phys ptr %#" PRIxPTR " ptr %p\n", len, ptr, paddr_to_physmap(ptr));

  return paddr_to_physmap(ptr);
}
#endif

// called from arch start.S
// run in physical space without the mmu set up, so stick to basic, relocatable code
extern "C" __NO_SAFESTACK
paddr_t boot_alloc_page_phys() {
  paddr_t ptr = ALIGN(boot_alloc_end, PAGE_SIZE);
  boot_alloc_end = ptr + PAGE_SIZE;

  return ptr;
}
