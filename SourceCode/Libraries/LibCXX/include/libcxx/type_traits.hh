#pragma once

#include "libcxx/std.hh"

NAMESPACE_STD_BEGIN

inline namespace _integral_constant {
/// integral_constant
template <typename _Tp, _Tp __v> struct integral_constant {
  static constexpr _Tp value = __v;
  using value_type           = _Tp;
  using type                 = integral_constant<_Tp, __v>;
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; }
};
} // namespace _integral_constant

inline namespace _true_false_type {
/// The type used as a compile-time boolean with true value.
using true_type = integral_constant<bool, true>;

/// The type used as a compile-time boolean with false value.
using false_type = integral_constant<bool, false>;
} // namespace _true_false_type

inline namespace _cv {
/// remove_cv
template <typename _Tp> struct remove_cv { using type = _Tp; };

template <typename _Tp> struct remove_cv<const _Tp> { using type = _Tp; };

template <typename _Tp> struct remove_cv<volatile _Tp> { using type = _Tp; };

template <typename _Tp> struct remove_cv<const volatile _Tp> { using type = _Tp; };

template <typename _Tp> using __remove_cv_t = typename remove_cv<_Tp>::type;
} // namespace _cv

inline namespace _is_integral {

template <typename> struct __is_integral_helper : public false_type {};

template <> struct __is_integral_helper<bool> : public true_type {};

template <> struct __is_integral_helper<char> : public true_type {};

template <> struct __is_integral_helper<signed char> : public true_type {};

template <> struct __is_integral_helper<unsigned char> : public true_type {};

#ifdef __WCHAR_TYPE__
template <> struct __is_integral_helper<wchar_t> : public true_type {};
#endif

template <> struct __is_integral_helper<char8_t> : public true_type {};

template <> struct __is_integral_helper<char16_t> : public true_type {};

template <> struct __is_integral_helper<char32_t> : public true_type {};

template <> struct __is_integral_helper<short> : public true_type {};

template <> struct __is_integral_helper<unsigned short> : public true_type {};

template <> struct __is_integral_helper<int> : public true_type {};

template <> struct __is_integral_helper<unsigned int> : public true_type {};

template <> struct __is_integral_helper<long> : public true_type {};

template <> struct __is_integral_helper<unsigned long> : public true_type {};

template <> struct __is_integral_helper<long long> : public true_type {};

template <> struct __is_integral_helper<unsigned long long> : public true_type {};

template <typename _Tp> struct is_integral : public __is_integral_helper<__remove_cv_t<_Tp>>::type {};

template <typename _Tp> inline constexpr bool is_integral_v = is_integral<_Tp>::value;
} // namespace _is_integral

inline namespace _const {
/// is_const
template <typename> struct is_const : public false_type {};

template <typename _Tp> struct is_const<const _Tp> : public true_type {};

template <typename _Tp> inline constexpr bool is_const_v = is_const<_Tp>::value;
} // namespace _const

NAMESPACE_STD_END // namespace std
