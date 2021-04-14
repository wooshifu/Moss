#include "libcxx/types.hh"

extern "C" {

// Samples taken at the first instruction in the kernel.
extern uint64_t kernel_entry_ticks[2]; // cntpct, cntvct
// ... and at the entry to normal virtual-space kernel code.
uint64_t kernel_virtual_entry_ticks[2]; // cntpct, cntvct

} // extern "C"
