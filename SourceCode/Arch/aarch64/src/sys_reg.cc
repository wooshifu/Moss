#include "aarch64/sys_reg.hh"

#include "libhal/cpu.hh" // for arch_curr_cpu_num, cpu_num_t

cpu_num_t arch_curr_cpu_num() { return sys_read<reg_mpidr_el1>().Aff0; }
