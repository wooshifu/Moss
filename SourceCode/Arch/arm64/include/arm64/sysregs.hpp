#pragma once

constexpr auto HCR_RW = (1 << 31);

constexpr auto HCR_HOST_NVHE_FLAGS = (HCR_RW);

constexpr auto SCTLR_ELx_M = (1UL << 0);

constexpr auto SCTLR_EE_LITTLE_ENDIAN   = (0 << 25);
constexpr auto SCTLR_EOE_LITTLE_ENDIAN  = (0 << 24);
constexpr auto SCTLR_MMU_DISABLED       = (0 << 0);
constexpr auto SCTLR_VALUE_MMU_DISABLED = (SCTLR_MMU_DISABLED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_EOE_LITTLE_ENDIAN);

constexpr auto SPSR_MASK_ALL = (7 << 6);
constexpr auto SPSR_EL1h     = (5 << 0);
constexpr auto SPSR_EL2h     = (9 << 0);
constexpr auto SPSR_EL1      = (SPSR_MASK_ALL | SPSR_EL1h);
constexpr auto SPSR_EL2      = (SPSR_MASK_ALL | SPSR_EL2h);

constexpr auto CurrentEL_EL1 = (1 << 2);
constexpr auto CurrentEL_EL2 = (2 << 2);
constexpr auto CurrentEL_EL3 = (3 << 2);
