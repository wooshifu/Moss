#include "aarch64/esr.hh"

/**
  Designated Initializers
  Standard C90 requires the elements of an initializer to appear in a fixed order, the same as the order of the elements
  in the array or structure being initialized.

  In ISO C99 you can give the elements in any order, specifying the array indices or structure field names they apply
  to, and GNU C allows this as an extension in C90 mode as well. This extension is not implemented in GNU C++.
 */
const char* esr_class_str[] = {
    [0 ... ESR_ELx_EC_MAX]   = "UNRECOGNIZED EC",
    [ESR_ELx_EC_UNKNOWN]     = "Unknown/Uncategorized",
    [ESR_ELx_EC_WFx]         = "WFI/WFE",
    [ESR_ELx_EC_CP15_32]     = "CP15 MCR/MRC",
    [ESR_ELx_EC_CP15_64]     = "CP15 MCRR/MRRC",
    [ESR_ELx_EC_CP14_MR]     = "CP14 MCR/MRC",
    [ESR_ELx_EC_CP14_LS]     = "CP14 LDC/STC",
    [ESR_ELx_EC_FP_ASIMD]    = "ASIMD",
    [ESR_ELx_EC_CP10_ID]     = "CP10 MRC/VMRS",
    [ESR_ELx_EC_PAC]         = "PAC",
    [ESR_ELx_EC_CP14_64]     = "CP14 MCRR/MRRC",
    [ESR_ELx_EC_BTI]         = "BTI",
    [ESR_ELx_EC_ILL]         = "PSTATE.IL",
    [ESR_ELx_EC_SVC32]       = "SVC (AArch32)",
    [ESR_ELx_EC_HVC32]       = "HVC (AArch32)",
    [ESR_ELx_EC_SMC32]       = "SMC (AArch32)",
    [ESR_ELx_EC_SVC64]       = "SVC (AArch64)",
    [ESR_ELx_EC_HVC64]       = "HVC (AArch64)",
    [ESR_ELx_EC_SMC64]       = "SMC (AArch64)",
    [ESR_ELx_EC_SYS64]       = "MSR/MRS (AArch64)",
    [ESR_ELx_EC_SVE]         = "SVE",
    [ESR_ELx_EC_ERET]        = "ERET/ERETAA/ERETAB",
    [ESR_ELx_EC_IMP_DEF]     = "EL3 IMP DEF",
    [ESR_ELx_EC_IABT_LOW]    = "IABT (lower EL)",
    [ESR_ELx_EC_IABT_CUR]    = "IABT (current EL)",
    [ESR_ELx_EC_PC_ALIGN]    = "PC Alignment",
    [ESR_ELx_EC_DABT_LOW]    = "DABT (lower EL)",
    [ESR_ELx_EC_DABT_CUR]    = "DABT (current EL)",
    [ESR_ELx_EC_SP_ALIGN]    = "SP Alignment",
    [ESR_ELx_EC_FP_EXC32]    = "FP (AArch32)",
    [ESR_ELx_EC_FP_EXC64]    = "FP (AArch64)",
    [ESR_ELx_EC_SERROR]      = "SError",
    [ESR_ELx_EC_BREAKPT_LOW] = "Breakpoint (lower EL)",
    [ESR_ELx_EC_BREAKPT_CUR] = "Breakpoint (current EL)",
    [ESR_ELx_EC_SOFTSTP_LOW] = "Software Step (lower EL)",
    [ESR_ELx_EC_SOFTSTP_CUR] = "Software Step (current EL)",
    [ESR_ELx_EC_WATCHPT_LOW] = "Watchpoint (lower EL)",
    [ESR_ELx_EC_WATCHPT_CUR] = "Watchpoint (current EL)",
    [ESR_ELx_EC_BKPT32]      = "BKPT (AArch32)",
    [ESR_ELx_EC_VECTOR32]    = "Vector catch (AArch32)",
    [ESR_ELx_EC_BRK64]       = "BRK (AArch64)",
};
