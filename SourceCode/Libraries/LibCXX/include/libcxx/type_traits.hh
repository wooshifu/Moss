#pragma once

#include "libcxx/cutting_line.hh"
#include "libcxx/std.hh"
#include "libcxx/types.hh"
#include "libcxx/utility.hh"

NAMESPACE_STD_BEGIN
________________________________________________________________________________________________________________________
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
________________________________________________________________________________________________________________________
inline namespace _true_false_type {
  /// The type used as a compile-time boolean with true value.
  using true_type = integral_constant<bool, true>;

  /// The type used as a compile-time boolean with false value.
  using false_type = integral_constant<bool, false>;
} // namespace _true_false_type
________________________________________________________________________________________________________________________
inline namespace _cv {
  /// remove_cv
  template <typename _Tp> struct remove_cv { using type = _Tp; };

  template <typename _Tp> struct remove_cv<const _Tp> { using type = _Tp; };

  template <typename _Tp> struct remove_cv<volatile _Tp> { using type = _Tp; };

  template <typename _Tp> struct remove_cv<const volatile _Tp> { using type = _Tp; };

  template <typename _Tp> using __remove_cv_t = typename remove_cv<_Tp>::type;
} // namespace _cv
________________________________________________________________________________________________________________________
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
________________________________________________________________________________________________________________________
inline namespace _const {
  /// is_const
  template <typename> struct is_const : public false_type {};

  template <typename _Tp> struct is_const<const _Tp> : public true_type {};

  template <typename _Tp> inline constexpr bool is_const_v = is_const<_Tp>::value;
} // namespace _const
________________________________________________________________________________________________________________________
inline namespace _enum {
  template <typename _Tp> struct is_enum : public integral_constant<bool, __is_enum(_Tp)> {};

  template <typename _Tp> inline constexpr bool is_enum_v = is_enum<_Tp>::value;
} // namespace _enum
________________________________________________________________________________________________________________________
inline namespace _underlying_type {
  template <typename _Tp, bool = is_enum_v<_Tp>> struct __underlying_type_impl { using type = __underlying_type(_Tp); };

  template <typename _Tp> struct __underlying_type_impl<_Tp, false> {};

  /// The underlying type of an enum.
  template <typename _Tp> struct underlying_type : public __underlying_type_impl<_Tp> {};

  /// Alias template for underlying_type
  template <typename _Tp> using underlying_type_t = typename underlying_type<_Tp>::type;

} // namespace _underlying_type
________________________________________________________________________________________________________________________
inline namespace _class {
  /// is_class
  template <typename _Tp> struct is_class : public integral_constant<bool, __is_class(_Tp)> {};
  template <typename _Tp> inline constexpr bool is_class_v = is_class<_Tp>::value;
} // namespace _class
________________________________________________________________________________________________________________________
inline namespace _conditional {
  template <bool, typename, typename> struct conditional;

  // Primary template.
  /// Define a member typedef @c type to one of two argument types.
  template <bool _Cond, typename _Iftrue, typename _Iffalse> struct conditional { using type = _Iftrue; };

  // Partial specialization for false.
  template <typename _Iftrue, typename _Iffalse> struct conditional<false, _Iftrue, _Iffalse> {
    using type = _Iffalse;
  };
  /// Alias template for conditional
  template <bool _Cond, typename _Iftrue, typename _Iffalse>
  using conditional_t = typename conditional<_Cond, _Iftrue, _Iffalse>::type;
} // namespace _conditional
________________________________________________________________________________________________________________________
inline namespace _or {
  template <typename...> struct __or_;

  template <> struct __or_<> : public false_type {};

  template <typename _B1> struct __or_<_B1> : public _B1 {};

  template <typename _B1, typename _B2> struct __or_<_B1, _B2> : public conditional<_B1::value, _B1, _B2>::type {};

  template <typename _B1, typename _B2, typename _B3, typename... _Bn>
  struct __or_<_B1, _B2, _B3, _Bn...> : public conditional<_B1::value, _B1, __or_<_B2, _B3, _Bn...>>::type {};
  template <typename... _Bn> inline constexpr bool __or_v = __or_<_Bn...>::value;
} // namespace _or
________________________________________________________________________________________________________________________
inline namespace _bool {
  template <bool __v> using __bool_constant = integral_constant<bool, __v>;
} // namespace _bool
________________________________________________________________________________________________________________________
inline namespace _void {
  template <typename> struct is_void;
  template <typename> struct __is_void_helper : public false_type {};

  template <> struct __is_void_helper<void> : public true_type {};

  /// is_void
  template <typename _Tp> struct is_void : public __is_void_helper<__remove_cv_t<_Tp>>::type {};
  template <typename _Tp> inline constexpr bool is_void_v = is_void<_Tp>::value;
} // namespace _void
________________________________________________________________________________________________________________________
inline namespace _function {
  template <typename> struct is_function;
  /// is_function
  template <typename _Tp> struct is_function : public __bool_constant<!is_const<const _Tp>::value> {};

  template <typename _Tp> struct is_function<_Tp&> : public false_type {};

  template <typename _Tp> struct is_function<_Tp&&> : public false_type {};

  template <typename _Tp> inline constexpr bool is_function_v = is_function<_Tp>::value;
} // namespace _function
________________________________________________________________________________________________________________________
inline namespace _array {
  /// is_array
  template <typename> struct is_array : public false_type {};

  template <typename _Tp, size_t _Size> struct is_array<_Tp[_Size]> : public true_type {};

  template <typename _Tp> struct is_array<_Tp[]> : public true_type {};
  template <typename _Tp> inline constexpr bool is_array_v = is_array<_Tp>::value;
} // namespace _array
________________________________________________________________________________________________________________________
inline namespace _convert {

  /// is_base_of
  template <typename _Base, typename _Derived>
  struct is_base_of : public integral_constant<bool, __is_base_of(_Base, _Derived)> {};

  template <typename _From, typename _To, bool = __or_<is_void<_From>, is_function<_To>, is_array<_To>>::value>
  struct __is_convertible_helper {
    using type = typename is_void<_To>::type;
  };

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
  template <typename _From, typename _To> class __is_convertible_helper<_From, _To, false> {
    template <typename _To1> static void __test_aux(_To1) noexcept;

    template <typename _From1, typename _To1, typename = decltype(__test_aux<_To1>(std::declval<_From1>()))>
    static true_type __test(int);

    template <typename, typename> static false_type __test(...);

  public:
    using type = decltype(__test<_From, _To>(0));
  };
#pragma GCC diagnostic pop

  /// is_convertible
  template <typename _From, typename _To> struct is_convertible : public __is_convertible_helper<_From, _To>::type {};

  // helper trait for unique_ptr<T[]>, shared_ptr<T[]>, and span<T, N>
  template <typename _ToElementType, typename _FromElementType>
  using __is_array_convertible = is_convertible<_FromElementType (*)[], _ToElementType (*)[]>;

  template <typename _From, typename _To, bool = __or_<is_void<_From>, is_function<_To>, is_array<_To>>::value>
  struct __is_nt_convertible_helper : is_void<_To> {};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
  template <typename _From, typename _To> class __is_nt_convertible_helper<_From, _To, false> {
    template <typename _To1> static void __test_aux(_To1) noexcept;

    template <typename _From1, typename _To1>
    static __bool_constant<noexcept(__test_aux<_To1>(std::declval<_From1>()))> __test(int);

    template <typename, typename> static false_type __test(...);

  public:
    using type = decltype(__test<_From, _To>(0));
  };
#pragma GCC diagnostic pop

  // is_nothrow_convertible for C++11
  template <typename _From, typename _To>
  struct __is_nothrow_convertible : public __is_nt_convertible_helper<_From, _To>::type {};

#if __cplusplus > 201703L
#define __cpp_lib_is_nothrow_convertible 201806L
  /// is_nothrow_convertible
  template <typename _From, typename _To>
  struct is_nothrow_convertible : public __is_nt_convertible_helper<_From, _To>::type {};

  /// is_nothrow_convertible_v
  template <typename _From, typename _To>
  inline constexpr bool is_nothrow_convertible_v = is_nothrow_convertible<_From, _To>::value;
#endif // C++2a

  template <typename _From, typename _To> inline constexpr bool is_convertible_v = is_convertible<_From, _To>::value;
} // namespace _convert
________________________________________________________________________________________________________________________
NAMESPACE_STD_END // namespace std
