constexpr auto CURRENTEL_EL1        = (0b01 << 2);
constexpr auto CURRENTEL_EL2        = (0b10 << 2);

constexpr auto CPACR_EL1_FPEN       = (0b11 << 20);
constexpr auto ID_AA64PFR0_EL1_GIC  = (0b1111 << 24);

constexpr auto CNTHCTL_EL2_EVNTEN   = (1 << 2);
constexpr auto CNTHCTL_EL2_EL1PCEN  = (1 << 1);
constexpr auto CNTHCTL_EL2_EL1PCTEN = (1 << 0);
constexpr auto CPTR_EL2_RES1        = 0x33ff;
constexpr auto HCR_EL2_RW           = (1 << 31);
constexpr auto ICC_SRE_EL2_SRE      = (1 << 0);
constexpr auto ICC_SRE_EL2_ENABLE   = (1 << 3);

constexpr auto SCR_EL3_HCE          = (1 << 8);
constexpr auto SCR_EL3_NS           = (1 << 0);
constexpr auto SCR_EL3_RW           = (1 << 10);

constexpr auto SPSR_ELX_DAIF        = (0b1111 << 6);
constexpr auto SPSR_ELX_EL1H        = (0b0101);

// constexpr auto ICH_HCR_EL2 = "S3_4_C12_C11_0";
// constexpr auto ICC_SRE_EL2 = "S3_4_C12_C9_5";

[[clang::optnone]] [[gnu::naked]] [[noreturn]] void _arm64_elX_to_el1() {
  asm volatile(
      R"(
.include "libstd/asm.hh"


FUNCTION arm64_elX_to_el1
    mrs x9, CurrentEL

    // Check the current exception level.
    cmp x9, %[CURRENTEL_EL1]
    beq .Ltarget
    cmp x9, %[CURRENTEL_EL2]
    beq .Lin_el2
    // Otherwise, we are in EL3.

    // Set EL2 to 64bit and enable the HVC instruction.
    mrs x9, scr_el3
    mov x10, %[SCR_EL3_NS] | %[SCR_EL3_HCE] | %[SCR_EL3_RW]
    orr x9, x9, x10
    msr scr_el3, x9

    // Set the return address and exception level.
    adr x9, .Ltarget
    msr elr_el3, x9
    mov x9, %[SPSR_ELX_DAIF] | %[SPSR_ELX_EL1H]
    msr spsr_el3, x9

.Lin_el2:
    // Set the init vector table for EL2.
    // todo: currently not configured
//    adr_global x9, arm64_el2_init_table
//    msr vbar_el2, x9

    mrs x9, cnthctl_el2
    // Disable EL1 timer traps and the timer offset.
    orr x9, x9, %[CNTHCTL_EL2_EL1PCEN] | %[CNTHCTL_EL2_EL1PCTEN]
    // Make sure the EL2 physical event stream is not running.
    bic x9, x9, %[CNTHCTL_EL2_EVNTEN]
    msr cnthctl_el2, x9
    msr cntvoff_el2, xzr

    // Disable stage 2 translations.
    msr vttbr_el2, xzr

    // Disable EL2 coprocessor traps.
    mov x9, %[CPTR_EL2_RES1]
    msr cptr_el2, x9

    // Disable EL1 FPU traps.
    mov x9, %[CPACR_EL1_FPEN]
    msr cpacr_el1, x9

    // Check whether the GIC system registers are supported.
    mrs x9, id_aa64pfr0_el1
    and x9, x9, %[ID_AA64PFR0_EL1_GIC]
    cbz x9, .Lno_gic_sr

    // Enable the GIC system registers in EL2, and allow their use in EL1.
    mrs x9, S3_4_C12_C9_5
    mov x10, %[ICC_SRE_EL2_ENABLE] | %[ICC_SRE_EL2_SRE]
    orr x9, x9, x10
    msr S3_4_C12_C9_5, x9

    // Disable the GIC virtual CPU interface.
    msr S3_4_C12_C11_0, xzr

.Lno_gic_sr:
    // Set EL1 to 64bit.
    mov x9, %[HCR_EL2_RW]
    msr hcr_el2, x9

    // Set the return address and exception level.
    adr x9, .Ltarget
    msr elr_el2, x9
    mov x9, %[SPSR_ELX_DAIF] | %[SPSR_ELX_EL1H]
    msr spsr_el2, x9

    isb
    eret

.Ltarget:
    ret
END_FUNCTION arm64_elX_to_el1
)"
      :
      :
      // clang-format off
        [CURRENTEL_EL1] "i"(CURRENTEL_EL1),
        [CURRENTEL_EL2] "i"(CURRENTEL_EL2),
        [SCR_EL3_NS] "i"(SCR_EL3_NS),
        [SCR_EL3_HCE] "i"(SCR_EL3_HCE),
        [SCR_EL3_RW] "i"(SCR_EL3_RW),
        [SPSR_ELX_DAIF] "i"(SPSR_ELX_DAIF),
        [SPSR_ELX_EL1H] "i"(SPSR_ELX_EL1H),
        [CNTHCTL_EL2_EL1PCEN] "i"(CNTHCTL_EL2_EL1PCEN),
        [CNTHCTL_EL2_EL1PCTEN] "i"(CNTHCTL_EL2_EL1PCTEN),
        [CNTHCTL_EL2_EVNTEN] "i"(CNTHCTL_EL2_EVNTEN),
        [ID_AA64PFR0_EL1_GIC] "i"(ID_AA64PFR0_EL1_GIC),
        [CPTR_EL2_RES1] "i"(CPTR_EL2_RES1),
        [CPACR_EL1_FPEN] "i"(CPACR_EL1_FPEN),
        [ICC_SRE_EL2_ENABLE] "i"(ICC_SRE_EL2_ENABLE),
        [ICC_SRE_EL2_SRE] "i"(ICC_SRE_EL2_SRE),
        [HCR_EL2_RW] "i"(HCR_EL2_RW)
      // clang-format on
      : "cc", "memory");
}
