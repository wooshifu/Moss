#include "aarch64/mp.hh"

// per cpu structures, each cpu will point to theirs using the fixed register
arm64_percpu arm64_percpu_array[SMP_MAX_CPUS];
