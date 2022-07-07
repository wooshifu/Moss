#pragma once

#include "libcxx/attr.hh"  // for attr_always_inline, attr_maybe_unused
#include "libcxx/types.hh" // for u64, u8

/* IWYU pragma: no_forward_declare reg_currentel */
/* IWYU pragma: no_forward_declare reg_sctlr_el1 */
/* IWYU pragma: no_forward_declare reg_sctlr_el1_t */

template <typename R, typename V> struct reg_t {
  static_assert(sizeof(R) == sizeof(V), "sizeof R and V must be same");

  reg_t() {} // NOLINT(modernize-use-equals-default)

  constexpr reg_t(const R& r) { reg = r; }
  constexpr reg_t(const V& v) { val = v; }

  union {
    R reg;
    V val;
  };
};

template <typename R> struct reg64_t : public reg_t<R, u64> {};
template <typename R> attr_always_inline R sys_read();
template <typename R> attr_always_inline void sys_write(const R& r);

#ifndef define_reg_ro
#define define_reg_ro(reg_name, reg_struct)                                                                            \
  struct attr_packed reg_##reg_name reg_struct;                                                                        \
  struct reg_##reg_name##_t : public reg64_t<reg_##reg_name> {};                                                       \
  template <> attr_always_inline reg_##reg_name sys_read<reg_##reg_name>() {                                           \
    reg_##reg_name reg{};                                                                                              \
    asm("mrs %[reg], " #reg_name : [reg] "=r"(reg));                                                                   \
    return reg;                                                                                                        \
  }                                                                                                                    \
                                                                                                                       \
  template <> attr_always_inline reg_##reg_name##_t sys_read<reg_##reg_name##_t>() {                                   \
    reg_##reg_name##_t reg{};                                                                                          \
    asm("mrs %[reg], " #reg_name : [reg] "=r"(reg));                                                                   \
    return reg;                                                                                                        \
  }
#endif

#ifndef define_reg_rw
#define define_reg_rw(reg_name, reg_struct)                                                                            \
  define_reg_ro(reg_name, reg_struct);                                                                                 \
                                                                                                                       \
  template <> attr_always_inline void sys_write(const reg_##reg_name& reg) {                                           \
    asm("msr " #reg_name ", %[reg]" : : [reg] "r"(reg));                                                               \
  }                                                                                                                    \
  template <> attr_always_inline void sys_write(const reg_##reg_name##_t& reg) {                                       \
    asm("msr " #reg_name ", %[reg]" : : [reg] "r"(reg.val));                                                           \
  }
#endif

define_reg_ro(currentel, {
  attr_maybe_unused u8 res0_0  : 2 = 0;
  u8 el                        : 2;
  attr_maybe_unused u64 res0_1 : 60 = 0;
});

// todo: use the correct sctlr_el1 struct body
define_reg_rw(sctlr_el1, { u64 x; });
