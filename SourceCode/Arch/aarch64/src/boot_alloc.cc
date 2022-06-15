#include "aarch64/boot_alloc.hh"

#include "aarch64/mmu.hh"
#include "kernel/aligh.hh"
#include "libcxx/attr.hh"
#include "libcxx/macros.hh"
#include "libcxx/types.hh"

extern char __code_start[];
extern char __code_end[];
extern char __rodata_start[];
extern char __rodata_end[];
extern char __data_start[];
extern char __data_end[];
extern char __bss_start[];
extern char _end[];

// store the start and current pointer to the boot allocator in physical address
paddr_t boot_alloc_start;
paddr_t boot_alloc_end;
// run in physical space without the mmu set up, so by computing the address of _end
// and saving it, we've effectively computed the physical address of the end of the
// kernel.
// We can't allow asan to check the globals here as it happens on a different
// aspace where asan shadow isn't mapped.
attr_no_asan attr_no_safestack extern_C void boot_alloc_init() {
  // todo: error, _end is at 0xffff'0000'0008'xxxx
  boot_alloc_start = reinterpret_cast<paddr_t>(_end);
  // TODO(fxbug.dev/32414): This is a compile-time no-op that defeats any compiler
  // optimizations based on its knowledge/assumption that `&_end` is a
  // constant here that equals the `&_end` constant as computed elsewhere.
  // Without this, the compiler can see that boot_alloc_start is never set to
  // any other value and replace code that uses the boot_alloc_start value
  // with code that computes `&_end` on the spot.  What the compiler doesn't
  // know is that this `&_end` is crucially a PC-relative computation when
  // the PC is a (low) physical address.  Later code that uses
  // boot_alloc_start will be running at a kernel (high) virtual address and
  // so its `&_end` will be nowhere near the same value.  The compiler isn't
  // wrong to do this sort of optimization when it can and other such cases
  // will eventually arise.  So long-term we'll need more thorough
  // compile-time separation of the early boot code that runs in physical
  // space from normal kernel code.  For now, this asm generates no
  // additional code but tells the compiler that it has no idea what value
  // boot_alloc_start might take, so it has to compute the `&_end` value now.
  __asm__("" : "=g"(boot_alloc_start) : "0"(boot_alloc_start));
  boot_alloc_end = reinterpret_cast<paddr_t>(_end);
}

// called from arch start.S
// run in physical space without the mmu set up, so stick to basic, relocatable code
attr_no_safestack paddr_t boot_alloc_page_phys() {
  paddr_t ptr    = ALIGN(boot_alloc_end, PAGE_SIZE);
  boot_alloc_end = ptr + PAGE_SIZE;

  return ptr;
}
