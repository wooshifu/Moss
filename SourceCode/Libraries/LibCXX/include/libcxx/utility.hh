#pragma once

#include "libcxx/std.hh"

NAMESPACE_STD_BEGIN

inline namespace _declval {
  template <typename _Tp> struct __declval_protector { static const bool __stop = false; };

  template <typename _Tp, typename _Up = _Tp&&> _Up __declval(int);

  template <typename _Tp> _Tp __declval(long);

  template <typename _Tp> auto declval() noexcept -> decltype(__declval<_Tp>(0));

  template <typename _Tp> auto declval() noexcept -> decltype(__declval<_Tp>(0)) {
    static_assert(__declval_protector<_Tp>::__stop, "declval() must not be used!");
    return __declval<_Tp>(0);
  }
} // namespace _declval

NAMESPACE_STD_END
