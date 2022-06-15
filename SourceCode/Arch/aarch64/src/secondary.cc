#include "aarch64/mp.hh"  // for SMP_MAX_CPUS
#include "libcxx/attr.hh" // for attr_naked, attr_noreturn, attr_optnone

attr_optnone attr_naked attr_noreturn void _arm64_get_secondary_sp() {
  asm volatile(
      R"(
.include "libstd/asm.hh"
.include "aarch64/asm_macros.hh"

FUNCTION arm64_get_secondary_sp
    mrs     x9, mpidr_el1
    movlit  x10, 0xff00ffffff  // Mask for AFFx (cluster) IDs.
    and     x9, x9, x10
    mov     x10, #%[SMP_MAX_CPUS]

    adr_g   x11, arm64_secondary_sp_list

.Lsp_loop:
    ldr     x12, [x11, #0]
    cmp     x12, x9
    beq     .Lsp_found
    add     x11, x11, #40
    subs    x10, x10, #1
    bne     .Lsp_loop
    mov     x0, xzr
    mov     x1, xzr
    mov     x2, xzr
    ret

.Lsp_found:
    ldr     x0, [x11, #8]
    ldr     x2, [x11, #16]
    add     x1, x11, #40
    ret

END_FUNCTION arm64_get_secondary_sp
       )"
      :
      : [SMP_MAX_CPUS] "i"(SMP_MAX_CPUS)
      : "cc", "memory");
}
