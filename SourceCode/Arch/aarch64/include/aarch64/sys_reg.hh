#pragma once

#include "libcxx/attr.hh"  // for attr_always_inline, attr_packed
#include "libcxx/types.hh" // for u8, u64, u32

// IWYU pragma: no_forward_declare reg_currentel
// IWYU pragma: no_forward_declare reg_sctlr_el1
// IWYU pragma: no_forward_declare reg_sctlr_el1_t
// IWYU pragma: no_forward_declare reg_mpidr_el1

template <typename R, typename V> struct reg_t {
  static_assert(sizeof(R) == sizeof(V), "sizeof R and V must be same");

  reg_t() : val{} {}

  constexpr explicit reg_t(const R& reg) { this->reg = reg; }
  constexpr explicit reg_t(const V& val) { this->val = val; }

  union {
    R reg;
    V val;
  };
};

template <typename R> struct reg64_t : public reg_t<R, u64> { using reg_t<R, u64>::reg_t; };
template <typename R> attr_always_inline inline R sys_read();
template <typename R> attr_always_inline inline void sys_write(const R& r);

#ifndef define_reg_ro
#define define_reg_ro(reg_name, reg_struct)                                                                            \
  struct attr_packed reg_##reg_name reg_struct;                                                                        \
  struct reg_##reg_name##_t : public reg64_t<reg_##reg_name> {                                                         \
    using reg64_t<reg_##reg_name>::reg64_t;                                                                            \
  };                                                                                                                   \
  template <> attr_always_inline inline reg_##reg_name sys_read<reg_##reg_name>() {                                    \
    reg_##reg_name reg{};                                                                                              \
    asm("mrs %[reg], " #reg_name : [reg] "=r"(reg));                                                                   \
    return reg;                                                                                                        \
  }                                                                                                                    \
                                                                                                                       \
  template <> attr_always_inline inline reg_##reg_name##_t sys_read<reg_##reg_name##_t>() {                            \
    reg_##reg_name##_t reg{};                                                                                          \
    asm("mrs %[reg], " #reg_name : [reg] "=r"(reg));                                                                   \
    return reg;                                                                                                        \
  }
#endif

#ifndef define_reg_rw
#define define_reg_rw(reg_name, reg_struct)                                                                            \
  define_reg_ro(reg_name, reg_struct);                                                                                 \
                                                                                                                       \
  template <> attr_always_inline inline void sys_write(const reg_##reg_name& reg) {                                    \
    asm("msr " #reg_name ", %[reg]" : : [reg] "r"(reg));                                                               \
  }                                                                                                                    \
  template <> attr_always_inline inline void sys_write(const reg_##reg_name##_t& reg) {                                \
    asm("msr " #reg_name ", %[reg]" : : [reg] "r"(reg.val));                                                           \
  }
#endif

define_reg_ro(currentel, {
  u8 RES0_0  : 2 = 0;  // bits[0:1]
  u8 EL      : 2;      // bits[2:3]
  u64 RES0_1 : 60 = 0; // bits[4:63]
});

define_reg_rw(sctlr_el1, {
  u8 M       : 1;     // bit [0]
  u8 A       : 1;     // bit [1]
  u8 C       : 1;     // bit [2]
  u8 SA      : 1;     // bit [3]
  u8 SA0     : 1;     // bit [4]
  u8 CP15BEN : 1;     // bit [5]
  u8 nAA     : 1;     // bit [6]
  u8 ITD     : 1;     // bit [7]
  u8 SED     : 1;     // bit [8]
  u8 UMA     : 1;     // bit [9]
  u8 EnRCTX  : 1;     // bit [10]
  u8 EOS     : 1;     // bit [11]
  u8 I       : 1;     // bit [12]
  u8 EnDB    : 1;     // bit [13]
  u8 DZE     : 1;     // bit [14]
  u8 UCT     : 1;     // bit [15]
  u8 nTWI    : 1;     // bit [16]
  u8 Bit     : 1;     // bit [17]
  u8 nTWE    : 1;     // bit [18]
  u8 WXN     : 1;     // bit [19]
  u8 TSCXT   : 1;     // bit [20]
  u8 IESB    : 1;     // bit [21]
  u8 EIS     : 1;     // bit [22]
  u8 SPAN    : 1;     // bit [23]
  u8 E0E     : 1;     // bit [24]
  u8 EE      : 1;     // bit [25]
  u8 UCI     : 1;     // bit [26]
  u8 EnDA    : 1;     // bit [27]
  u8 nTLSMD  : 1;     // bit [28]
  u8 LSMAOE  : 1;     // bit [29]
  u8 EnIB    : 1;     // bit [30]
  u8 EnIA    : 1;     // bit [31]
  u8 RES0_0  : 3 = 0; // bits [34:32]
  u8 BT0     : 1;     // bit [35]
  u8 BT1     : 1;     // bit [36]
  u8 ITFSB   : 1;     // bit [37]
  u8 TCF0    : 2;     // bits [39:38]
  u8 TCF     : 2;     // bits [41:40]
  u8 ATA0    : 1;     // bit [42]
  u8 ATA     : 1;     // bit [43]
  u8 DSSBS   : 1;     // bit [44]
  u8 TWEDEn  : 1;     // bit [45]
  u8 TWEDEL  : 4;     // bits [49:46]
  u8 RES0_1  : 4 = 0; // bits [53:50]
  u8 EnASR   : 1;     // bit [54]
  u8 EnAS0   : 1;     // bit [55]
  u8 EnALS   : 1;     // bit [56]
  u8 EPAN    : 1;     // bit [57]
  u8 RES0_2  : 6 = 0; // bits [63:58]
});

define_reg_ro(mpidr_el1, {
  u8 Aff0    : 8;      // bits [7:0]
  u8 Aff1    : 8;      // bits [15:8]
  u8 Aff2    : 8;      // bits [23:16]
  u8 MT      : 1;      // bit [24]
  u8 RES0_0  : 5 = 0;  // bits [29:25]
  u8 U       : 1;      // bit [30]
  u8 RES1    : 1 = 1;  // bit [31]
  u8 Aff3    : 8;      // bits [39:32]
  u32 RES0_1 : 24 = 0; // bits [63:40]
});
