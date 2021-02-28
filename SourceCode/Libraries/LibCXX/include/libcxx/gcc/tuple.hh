// <tuple> -*- C++ -*-

// Copyright (C) 2007-2020 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file include/tuple
 *  This is a Standard C++ Library header.
 */

#ifndef _GLIBCXX_TUPLE
#define _GLIBCXX_TUPLE 1

#pragma GCC system_header

#if __cplusplus < 201103L
#include "libcxx/gcc/bits/c++0x_warning.hh"
#else

#include "libcxx/gcc/utility.hh"
#include "libcxx/gcc/array.hh"
#include "libcxx/gcc/bits/uses_allocator.hh"
#include "libcxx/gcc/bits/invoke.hh"
#if __cplusplus > 201703L
#include "libcxx/gcc/compare.hh"
#define __cpp_lib_constexpr_tuple 201811L
#endif

namespace std _GLIBCXX_VISIBILITY(default) {
  _GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @addtogroup utilities
   *  @{
   */

  template <typename... _Elements> class tuple;

  template <typename _Tp> struct __is_empty_non_tuple : is_empty<_Tp> {};

  // Using EBO for elements that are tuples causes ambiguous base errors.
  template <typename _El0, typename... _El> struct __is_empty_non_tuple<tuple<_El0, _El...>> : false_type {};

  // Use the Empty Base-class Optimization for empty, non-final types.
  template <typename _Tp>
  using __empty_not_final = typename conditional<__is_final(_Tp), false_type, __is_empty_non_tuple<_Tp>>::type;

  template <std::size_t _Idx, typename _Head, bool = __empty_not_final<_Head>::value> struct _Head_base;

  template <std::size_t _Idx, typename _Head> struct _Head_base<_Idx, _Head, true> : public _Head {
    constexpr _Head_base() : _Head() {}

    constexpr _Head_base(const _Head& __h) : _Head(__h) {}

    constexpr _Head_base(const _Head_base&) = default;
    constexpr _Head_base(_Head_base&&)      = default;

    template <typename _UHead> constexpr _Head_base(_UHead&& __h) : _Head(std::forward<_UHead>(__h)) {}

    _Head_base(allocator_arg_t, __uses_alloc0) : _Head() {}

    template <typename _Alloc>
    _Head_base(allocator_arg_t, __uses_alloc1<_Alloc> __a) : _Head(allocator_arg, *__a._M_a) {}

    template <typename _Alloc> _Head_base(allocator_arg_t, __uses_alloc2<_Alloc> __a) : _Head(*__a._M_a) {}

    template <typename _UHead> _Head_base(__uses_alloc0, _UHead&& __uhead) : _Head(std::forward<_UHead>(__uhead)) {}

    template <typename _Alloc, typename _UHead>
    _Head_base(__uses_alloc1<_Alloc> __a, _UHead&& __uhead)
        : _Head(allocator_arg, *__a._M_a, std::forward<_UHead>(__uhead)) {}

    template <typename _Alloc, typename _UHead>
    _Head_base(__uses_alloc2<_Alloc> __a, _UHead&& __uhead) : _Head(std::forward<_UHead>(__uhead), *__a._M_a) {}

    static constexpr _Head& _M_head(_Head_base& __b) noexcept { return __b; }

    static constexpr const _Head& _M_head(const _Head_base& __b) noexcept { return __b; }
  };

  template <std::size_t _Idx, typename _Head> struct _Head_base<_Idx, _Head, false> {
    constexpr _Head_base() : _M_head_impl() {}

    constexpr _Head_base(const _Head& __h) : _M_head_impl(__h) {}

    constexpr _Head_base(const _Head_base&) = default;
    constexpr _Head_base(_Head_base&&)      = default;

    template <typename _UHead> constexpr _Head_base(_UHead&& __h) : _M_head_impl(std::forward<_UHead>(__h)) {}

    _GLIBCXX20_CONSTEXPR
    _Head_base(allocator_arg_t, __uses_alloc0) : _M_head_impl() {}

    template <typename _Alloc>
    _Head_base(allocator_arg_t, __uses_alloc1<_Alloc> __a) : _M_head_impl(allocator_arg, *__a._M_a) {}

    template <typename _Alloc> _Head_base(allocator_arg_t, __uses_alloc2<_Alloc> __a) : _M_head_impl(*__a._M_a) {}

    template <typename _UHead>
    _GLIBCXX20_CONSTEXPR _Head_base(__uses_alloc0, _UHead&& __uhead) : _M_head_impl(std::forward<_UHead>(__uhead)) {}

    template <typename _Alloc, typename _UHead>
    _Head_base(__uses_alloc1<_Alloc> __a, _UHead&& __uhead)
        : _M_head_impl(allocator_arg, *__a._M_a, std::forward<_UHead>(__uhead)) {}

    template <typename _Alloc, typename _UHead>
    _Head_base(__uses_alloc2<_Alloc> __a, _UHead&& __uhead) : _M_head_impl(std::forward<_UHead>(__uhead), *__a._M_a) {}

    static constexpr _Head& _M_head(_Head_base& __b) noexcept { return __b._M_head_impl; }

    static constexpr const _Head& _M_head(const _Head_base& __b) noexcept { return __b._M_head_impl; }

    _Head _M_head_impl;
  };

  /**
   * Contains the actual implementation of the @c tuple template, stored
   * as a recursive inheritance hierarchy from the first element (most
   * derived class) to the last (least derived class). The @c Idx
   * parameter gives the 0-based index of the element stored at this
   * point in the hierarchy; we use it to implement a constant-time
   * get() operation.
   */
  template <std::size_t _Idx, typename... _Elements> struct _Tuple_impl;

  /**
   * Recursive tuple implementation. Here we store the @c Head element
   * and derive from a @c Tuple_impl containing the remaining elements
   * (which contains the @c Tail).
   */
  template <std::size_t _Idx, typename _Head, typename... _Tail>
  struct _Tuple_impl<_Idx, _Head, _Tail...> : public _Tuple_impl<_Idx + 1, _Tail...>, private _Head_base<_Idx, _Head> {
    template <std::size_t, typename...> friend class _Tuple_impl;

    typedef _Tuple_impl<_Idx + 1, _Tail...> _Inherited;
    typedef _Head_base<_Idx, _Head> _Base;

    static constexpr _Head& _M_head(_Tuple_impl& __t) noexcept { return _Base::_M_head(__t); }

    static constexpr const _Head& _M_head(const _Tuple_impl& __t) noexcept { return _Base::_M_head(__t); }

    static constexpr _Inherited& _M_tail(_Tuple_impl& __t) noexcept { return __t; }

    static constexpr const _Inherited& _M_tail(const _Tuple_impl& __t) noexcept { return __t; }

    constexpr _Tuple_impl() : _Inherited(), _Base() {}

    explicit constexpr _Tuple_impl(const _Head& __head, const _Tail&... __tail)
        : _Inherited(__tail...), _Base(__head) {}

    template <typename _UHead, typename... _UTail,
              typename = typename enable_if<sizeof...(_Tail) == sizeof...(_UTail)>::type>
    explicit constexpr _Tuple_impl(_UHead&& __head, _UTail&&... __tail)
        : _Inherited(std::forward<_UTail>(__tail)...), _Base(std::forward<_UHead>(__head)) {}

    constexpr _Tuple_impl(const _Tuple_impl&) = default;

    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 2729. Missing SFINAE on std::pair::operator=
    _Tuple_impl& operator=(const _Tuple_impl&) = delete;

    constexpr _Tuple_impl(_Tuple_impl&& __in) noexcept(
        __and_<is_nothrow_move_constructible<_Head>, is_nothrow_move_constructible<_Inherited>>::value)
        : _Inherited(std::move(_M_tail(__in))), _Base(std::forward<_Head>(_M_head(__in))) {}

    template <typename... _UElements>
    constexpr _Tuple_impl(const _Tuple_impl<_Idx, _UElements...>& __in)
        : _Inherited(_Tuple_impl<_Idx, _UElements...>::_M_tail(__in)),
          _Base(_Tuple_impl<_Idx, _UElements...>::_M_head(__in)) {}

    template <typename _UHead, typename... _UTails>
    constexpr _Tuple_impl(_Tuple_impl<_Idx, _UHead, _UTails...>&& __in)
        : _Inherited(std::move(_Tuple_impl<_Idx, _UHead, _UTails...>::_M_tail(__in))),
          _Base(std::forward<_UHead>(_Tuple_impl<_Idx, _UHead, _UTails...>::_M_head(__in))) {}

    template <typename _Alloc>
    _GLIBCXX20_CONSTEXPR _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a)
        : _Inherited(__tag, __a), _Base(__tag, __use_alloc<_Head>(__a)) {}

    template <typename _Alloc>
    _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, const _Head& __head, const _Tail&... __tail)
        : _Inherited(__tag, __a, __tail...), _Base(__use_alloc<_Head, _Alloc, _Head>(__a), __head) {}

    template <typename _Alloc, typename _UHead, typename... _UTail,
              typename = typename enable_if<sizeof...(_Tail) == sizeof...(_UTail)>::type>
    _GLIBCXX20_CONSTEXPR _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, _UHead&& __head, _UTail&&... __tail)
        : _Inherited(__tag, __a, std::forward<_UTail>(__tail)...),
          _Base(__use_alloc<_Head, _Alloc, _UHead>(__a), std::forward<_UHead>(__head)) {}

    template <typename _Alloc>
    _GLIBCXX20_CONSTEXPR _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, const _Tuple_impl& __in)
        : _Inherited(__tag, __a, _M_tail(__in)), _Base(__use_alloc<_Head, _Alloc, _Head>(__a), _M_head(__in)) {}

    template <typename _Alloc>
    _GLIBCXX20_CONSTEXPR _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, _Tuple_impl&& __in)
        : _Inherited(__tag, __a, std::move(_M_tail(__in))),
          _Base(__use_alloc<_Head, _Alloc, _Head>(__a), std::forward<_Head>(_M_head(__in))) {}

    template <typename _Alloc, typename... _UElements>
    _GLIBCXX20_CONSTEXPR _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a,
                                     const _Tuple_impl<_Idx, _UElements...>& __in)
        : _Inherited(__tag, __a, _Tuple_impl<_Idx, _UElements...>::_M_tail(__in)),
          _Base(__use_alloc<_Head, _Alloc, _Head>(__a), _Tuple_impl<_Idx, _UElements...>::_M_head(__in)) {}

    template <typename _Alloc, typename _UHead, typename... _UTails>
    _GLIBCXX20_CONSTEXPR _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a,
                                     _Tuple_impl<_Idx, _UHead, _UTails...>&& __in)
        : _Inherited(__tag, __a, std::move(_Tuple_impl<_Idx, _UHead, _UTails...>::_M_tail(__in))),
          _Base(__use_alloc<_Head, _Alloc, _UHead>(__a),
                std::forward<_UHead>(_Tuple_impl<_Idx, _UHead, _UTails...>::_M_head(__in))) {}

    template <typename... _UElements>
    _GLIBCXX20_CONSTEXPR void _M_assign(const _Tuple_impl<_Idx, _UElements...>& __in) {
      _M_head(*this) = _Tuple_impl<_Idx, _UElements...>::_M_head(__in);
      _M_tail(*this)._M_assign(_Tuple_impl<_Idx, _UElements...>::_M_tail(__in));
    }

    template <typename _UHead, typename... _UTails>
    _GLIBCXX20_CONSTEXPR void _M_assign(_Tuple_impl<_Idx, _UHead, _UTails...>&& __in) {
      _M_head(*this) = std::forward<_UHead>(_Tuple_impl<_Idx, _UHead, _UTails...>::_M_head(__in));
      _M_tail(*this)._M_assign(std::move(_Tuple_impl<_Idx, _UHead, _UTails...>::_M_tail(__in)));
    }

  protected:
    _GLIBCXX20_CONSTEXPR
    void _M_swap(_Tuple_impl& __in) {
      using std::swap;
      swap(_M_head(*this), _M_head(__in));
      _Inherited::_M_swap(_M_tail(__in));
    }
  };

  // Basis case of inheritance recursion.
  template <std::size_t _Idx, typename _Head> struct _Tuple_impl<_Idx, _Head> : private _Head_base<_Idx, _Head> {
    template <std::size_t, typename...> friend class _Tuple_impl;

    typedef _Head_base<_Idx, _Head> _Base;

    static constexpr _Head& _M_head(_Tuple_impl& __t) noexcept { return _Base::_M_head(__t); }

    static constexpr const _Head& _M_head(const _Tuple_impl& __t) noexcept { return _Base::_M_head(__t); }

    constexpr _Tuple_impl() : _Base() {}

    explicit constexpr _Tuple_impl(const _Head& __head) : _Base(__head) {}

    template <typename _UHead> explicit constexpr _Tuple_impl(_UHead&& __head) : _Base(std::forward<_UHead>(__head)) {}

    constexpr _Tuple_impl(const _Tuple_impl&) = default;

    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 2729. Missing SFINAE on std::pair::operator=
    _Tuple_impl& operator=(const _Tuple_impl&) = delete;

    constexpr _Tuple_impl(_Tuple_impl&& __in) noexcept(is_nothrow_move_constructible<_Head>::value)
        : _Base(std::forward<_Head>(_M_head(__in))) {}

    template <typename _UHead>
    constexpr _Tuple_impl(const _Tuple_impl<_Idx, _UHead>& __in) : _Base(_Tuple_impl<_Idx, _UHead>::_M_head(__in)) {}

    template <typename _UHead>
    constexpr _Tuple_impl(_Tuple_impl<_Idx, _UHead>&& __in)
        : _Base(std::forward<_UHead>(_Tuple_impl<_Idx, _UHead>::_M_head(__in))) {}

    template <typename _Alloc>
    _GLIBCXX20_CONSTEXPR _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a)
        : _Base(__tag, __use_alloc<_Head>(__a)) {}

    template <typename _Alloc>
    _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, const _Head& __head)
        : _Base(__use_alloc<_Head, _Alloc, _Head>(__a), __head) {}

    template <typename _Alloc, typename _UHead>
    _GLIBCXX20_CONSTEXPR _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, _UHead&& __head)
        : _Base(__use_alloc<_Head, _Alloc, _UHead>(__a), std::forward<_UHead>(__head)) {}

    template <typename _Alloc>
    _GLIBCXX20_CONSTEXPR _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, const _Tuple_impl& __in)
        : _Base(__use_alloc<_Head, _Alloc, _Head>(__a), _M_head(__in)) {}

    template <typename _Alloc>
    _GLIBCXX20_CONSTEXPR _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, _Tuple_impl&& __in)
        : _Base(__use_alloc<_Head, _Alloc, _Head>(__a), std::forward<_Head>(_M_head(__in))) {}

    template <typename _Alloc, typename _UHead>
    _GLIBCXX20_CONSTEXPR _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, const _Tuple_impl<_Idx, _UHead>& __in)
        : _Base(__use_alloc<_Head, _Alloc, _Head>(__a), _Tuple_impl<_Idx, _UHead>::_M_head(__in)) {}

    template <typename _Alloc, typename _UHead>
    _GLIBCXX20_CONSTEXPR _Tuple_impl(allocator_arg_t __tag, const _Alloc& __a, _Tuple_impl<_Idx, _UHead>&& __in)
        : _Base(__use_alloc<_Head, _Alloc, _UHead>(__a),
                std::forward<_UHead>(_Tuple_impl<_Idx, _UHead>::_M_head(__in))) {}

    template <typename _UHead> _GLIBCXX20_CONSTEXPR void _M_assign(const _Tuple_impl<_Idx, _UHead>& __in) {
      _M_head(*this) = _Tuple_impl<_Idx, _UHead>::_M_head(__in);
    }

    template <typename _UHead> _GLIBCXX20_CONSTEXPR void _M_assign(_Tuple_impl<_Idx, _UHead>&& __in) {
      _M_head(*this) = std::forward<_UHead>(_Tuple_impl<_Idx, _UHead>::_M_head(__in));
    }

  protected:
    _GLIBCXX20_CONSTEXPR
    void _M_swap(_Tuple_impl& __in) {
      using std::swap;
      swap(_M_head(*this), _M_head(__in));
    }
  };

  // Concept utility functions, reused in conditionally-explicit
  // constructors.
  template <bool, typename... _Types> struct _TupleConstraints {
    // Constraint for a non-explicit constructor.
    // True iff each Ti in _Types... can be constructed from Ui in _UTypes...
    // and every Ui is implicitly convertible to Ti.
    template <typename... _UTypes> static constexpr bool __is_implicitly_constructible() {
      return __and_<is_constructible<_Types, _UTypes>..., is_convertible<_UTypes, _Types>...>::value;
    }

    // Constraint for a non-explicit constructor.
    // True iff each Ti in _Types... can be constructed from Ui in _UTypes...
    // but not every Ui is implicitly convertible to Ti.
    template <typename... _UTypes> static constexpr bool __is_explicitly_constructible() {
      return __and_<is_constructible<_Types, _UTypes>..., __not_<__and_<is_convertible<_UTypes, _Types>...>>>::value;
    }

    static constexpr bool __is_implicitly_default_constructible() {
      return __and_<std::__is_implicitly_default_constructible<_Types>...>::value;
    }

    static constexpr bool __is_explicitly_default_constructible() {
      return __and_<is_default_constructible<_Types>...,
                    __not_<__and_<std::__is_implicitly_default_constructible<_Types>...>>>::value;
    }
  };

  // Partial specialization used when a required precondition isn't met,
  // e.g. when sizeof...(_Types) != sizeof...(_UTypes).
  template <typename... _Types> struct _TupleConstraints<false, _Types...> {
    template <typename... _UTypes> static constexpr bool __is_implicitly_constructible() { return false; }

    template <typename... _UTypes> static constexpr bool __is_explicitly_constructible() { return false; }
  };

  /// Primary class template, tuple
  template <typename... _Elements> class tuple : public _Tuple_impl<0, _Elements...> {
    typedef _Tuple_impl<0, _Elements...> _Inherited;

    template <bool _Cond> using _TCC = _TupleConstraints<_Cond, _Elements...>;

    // Constraint for non-explicit default constructor
    template <bool _Dummy>
    using _ImplicitDefaultCtor = __enable_if_t<_TCC<_Dummy>::__is_implicitly_default_constructible(), bool>;

    // Constraint for explicit default constructor
    template <bool _Dummy>
    using _ExplicitDefaultCtor = __enable_if_t<_TCC<_Dummy>::__is_explicitly_default_constructible(), bool>;

    // Constraint for non-explicit constructors
    template <bool _Cond, typename... _Args>
    using _ImplicitCtor = __enable_if_t<_TCC<_Cond>::template __is_implicitly_constructible<_Args...>(), bool>;

    // Constraint for non-explicit constructors
    template <bool _Cond, typename... _Args>
    using _ExplicitCtor = __enable_if_t<_TCC<_Cond>::template __is_explicitly_constructible<_Args...>(), bool>;

    template <typename... _UElements>
    static constexpr __enable_if_t<sizeof...(_UElements) == sizeof...(_Elements), bool> __assignable() {
      return __and_<is_assignable<_Elements&, _UElements>...>::value;
    }

    // Condition for noexcept-specifier of an assignment operator.
    template <typename... _UElements> static constexpr bool __nothrow_assignable() {
      return __and_<is_nothrow_assignable<_Elements&, _UElements>...>::value;
    }

    // Condition for noexcept-specifier of a constructor.
    template <typename... _UElements> static constexpr bool __nothrow_constructible() {
      return __and_<is_nothrow_constructible<_Elements, _UElements>...>::value;
    }

    // Constraint for tuple(_UTypes&&...) where sizeof...(_UTypes) == 1.
    template <typename _Up> static constexpr bool __valid_args() {
      return sizeof...(_Elements) == 1 && !is_same<tuple, __remove_cvref_t<_Up>>::value;
    }

    // Constraint for tuple(_UTypes&&...) where sizeof...(_UTypes) > 1.
    template <typename, typename, typename... _Tail> static constexpr bool __valid_args() {
      return (sizeof...(_Tail) + 2) == sizeof...(_Elements);
    }

    /* Constraint for constructors with a tuple<UTypes...> parameter ensures
     * that the constructor is only viable when it would not interfere with
     * tuple(UTypes&&...) or tuple(const tuple&) or tuple(tuple&&).
     * Such constructors are only viable if:
     * either sizeof...(Types) != 1,
     * or (when Types... expands to T and UTypes... expands to U)
     * is_convertible_v<TUPLE, T>, is_constructible_v<T, TUPLE>,
     * and is_same_v<T, U> are all false.
     */
    template <typename _Tuple, typename = tuple, typename = __remove_cvref_t<_Tuple>>
    struct _UseOtherCtor : false_type {};
    // If TUPLE is convertible to the single element in *this,
    // then TUPLE should match tuple(UTypes&&...) instead.
    template <typename _Tuple, typename _Tp, typename _Up>
    struct _UseOtherCtor<_Tuple, tuple<_Tp>, tuple<_Up>>
        : __or_<is_convertible<_Tuple, _Tp>, is_constructible<_Tp, _Tuple>> {};
    // If TUPLE and *this each have a single element of the same type,
    // then TUPLE should match a copy/move constructor instead.
    template <typename _Tuple, typename _Tp> struct _UseOtherCtor<_Tuple, tuple<_Tp>, tuple<_Tp>> : true_type {};

    // Return true iff sizeof...(Types) == 1 && tuple_size_v<TUPLE> == 1
    // and the single element in Types can be initialized from TUPLE,
    // or is the same type as tuple_element_t<0, TUPLE>.
    template <typename _Tuple> static constexpr bool __use_other_ctor() { return _UseOtherCtor<_Tuple>::value; }

  public:
    template <typename _Dummy = void, _ImplicitDefaultCtor<is_void<_Dummy>::value> = true>
    constexpr tuple() noexcept(__and_<is_nothrow_default_constructible<_Elements>...>::value) : _Inherited() {}

    template <typename _Dummy = void, _ExplicitDefaultCtor<is_void<_Dummy>::value> = false>
    explicit constexpr tuple() noexcept(__and_<is_nothrow_default_constructible<_Elements>...>::value) : _Inherited() {}

    template <bool _NotEmpty = (sizeof...(_Elements) >= 1), _ImplicitCtor<_NotEmpty, const _Elements&...> = true>
    constexpr tuple(const _Elements&... __elements) noexcept(__nothrow_constructible<const _Elements&...>())
        : _Inherited(__elements...) {}

    template <bool _NotEmpty = (sizeof...(_Elements) >= 1), _ExplicitCtor<_NotEmpty, const _Elements&...> = false>
    explicit constexpr tuple(const _Elements&... __elements) noexcept(__nothrow_constructible<const _Elements&...>())
        : _Inherited(__elements...) {}

    template <typename... _UElements, bool _Valid = __valid_args<_UElements...>(),
              _ImplicitCtor<_Valid, _UElements...> = true>
    constexpr tuple(_UElements&&... __elements) noexcept(__nothrow_constructible<_UElements...>())
        : _Inherited(std::forward<_UElements>(__elements)...) {}

    template <typename... _UElements, bool _Valid = __valid_args<_UElements...>(),
              _ExplicitCtor<_Valid, _UElements...> = false>
    explicit constexpr tuple(_UElements&&... __elements) noexcept(__nothrow_constructible<_UElements...>())
        : _Inherited(std::forward<_UElements>(__elements)...) {}

    constexpr tuple(const tuple&) = default;

    constexpr tuple(tuple&&) = default;

    template <typename... _UElements,
              bool _Valid = (sizeof...(_Elements) == sizeof...(_UElements)) &&
                            !__use_other_ctor<const tuple<_UElements...>&>(),
              _ImplicitCtor<_Valid, const _UElements&...> = true>
    constexpr tuple(const tuple<_UElements...>& __in) noexcept(__nothrow_constructible<const _UElements&...>())
        : _Inherited(static_cast<const _Tuple_impl<0, _UElements...>&>(__in)) {}

    template <typename... _UElements,
              bool _Valid = (sizeof...(_Elements) == sizeof...(_UElements)) &&
                            !__use_other_ctor<const tuple<_UElements...>&>(),
              _ExplicitCtor<_Valid, const _UElements&...> = false>
    explicit constexpr tuple(const tuple<_UElements...>& __in) noexcept(__nothrow_constructible<const _UElements&...>())
        : _Inherited(static_cast<const _Tuple_impl<0, _UElements...>&>(__in)) {}

    template <typename... _UElements,
              bool _Valid = (sizeof...(_Elements) == sizeof...(_UElements)) &&
                            !__use_other_ctor<tuple<_UElements...>&&>(),
              _ImplicitCtor<_Valid, _UElements...> = true>
    constexpr tuple(tuple<_UElements...>&& __in) noexcept(__nothrow_constructible<_UElements...>())
        : _Inherited(static_cast<_Tuple_impl<0, _UElements...>&&>(__in)) {}

    template <typename... _UElements,
              bool _Valid = (sizeof...(_Elements) == sizeof...(_UElements)) &&
                            !__use_other_ctor<tuple<_UElements...>&&>(),
              _ExplicitCtor<_Valid, _UElements...> = false>
    explicit constexpr tuple(tuple<_UElements...>&& __in) noexcept(__nothrow_constructible<_UElements...>())
        : _Inherited(static_cast<_Tuple_impl<0, _UElements...>&&>(__in)) {}

    // Allocator-extended constructors.

    template <typename _Alloc, _ImplicitDefaultCtor<is_object<_Alloc>::value> = true>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a) : _Inherited(__tag, __a) {}

    template <typename _Alloc, bool _NotEmpty = (sizeof...(_Elements) >= 1),
              _ImplicitCtor<_NotEmpty, const _Elements&...> = true>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, const _Elements&... __elements)
        : _Inherited(__tag, __a, __elements...) {}

    template <typename _Alloc, bool _NotEmpty = (sizeof...(_Elements) >= 1),
              _ExplicitCtor<_NotEmpty, const _Elements&...> = false>
    _GLIBCXX20_CONSTEXPR explicit tuple(allocator_arg_t __tag, const _Alloc& __a, const _Elements&... __elements)
        : _Inherited(__tag, __a, __elements...) {}

    template <typename _Alloc, typename... _UElements, bool _Valid = __valid_args<_UElements...>(),
              _ImplicitCtor<_Valid, _UElements...> = true>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, _UElements&&... __elements)
        : _Inherited(__tag, __a, std::forward<_UElements>(__elements)...) {}

    template <typename _Alloc, typename... _UElements, bool _Valid = __valid_args<_UElements...>(),
              _ExplicitCtor<_Valid, _UElements...> = false>
    _GLIBCXX20_CONSTEXPR explicit tuple(allocator_arg_t __tag, const _Alloc& __a, _UElements&&... __elements)
        : _Inherited(__tag, __a, std::forward<_UElements>(__elements)...) {}

    template <typename _Alloc>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, const tuple& __in)
        : _Inherited(__tag, __a, static_cast<const _Inherited&>(__in)) {}

    template <typename _Alloc>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, tuple&& __in)
        : _Inherited(__tag, __a, static_cast<_Inherited&&>(__in)) {}

    template <typename _Alloc, typename... _UElements,
              bool _Valid = (sizeof...(_Elements) == sizeof...(_UElements)) &&
                            !__use_other_ctor<const tuple<_UElements...>&>(),
              _ImplicitCtor<_Valid, const _UElements&...> = true>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, const tuple<_UElements...>& __in)
        : _Inherited(__tag, __a, static_cast<const _Tuple_impl<0, _UElements...>&>(__in)) {}

    template <typename _Alloc, typename... _UElements,
              bool _Valid = (sizeof...(_Elements) == sizeof...(_UElements)) &&
                            !__use_other_ctor<const tuple<_UElements...>&>(),
              _ExplicitCtor<_Valid, const _UElements&...> = false>
    _GLIBCXX20_CONSTEXPR explicit tuple(allocator_arg_t __tag, const _Alloc& __a, const tuple<_UElements...>& __in)
        : _Inherited(__tag, __a, static_cast<const _Tuple_impl<0, _UElements...>&>(__in)) {}

    template <typename _Alloc, typename... _UElements,
              bool _Valid = (sizeof...(_Elements) == sizeof...(_UElements)) &&
                            !__use_other_ctor<tuple<_UElements...>&&>(),
              _ImplicitCtor<_Valid, _UElements...> = true>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, tuple<_UElements...>&& __in)
        : _Inherited(__tag, __a, static_cast<_Tuple_impl<0, _UElements...>&&>(__in)) {}

    template <typename _Alloc, typename... _UElements,
              bool _Valid = (sizeof...(_Elements) == sizeof...(_UElements)) &&
                            !__use_other_ctor<tuple<_UElements...>&&>(),
              _ExplicitCtor<_Valid, _UElements...> = false>
    _GLIBCXX20_CONSTEXPR explicit tuple(allocator_arg_t __tag, const _Alloc& __a, tuple<_UElements...>&& __in)
        : _Inherited(__tag, __a, static_cast<_Tuple_impl<0, _UElements...>&&>(__in)) {}

    // tuple assignment

    _GLIBCXX20_CONSTEXPR
    tuple& operator=(typename conditional<__assignable<const _Elements&...>(), const tuple&, const __nonesuch&>::type
                         __in) noexcept(__nothrow_assignable<const _Elements&...>()) {
      this->_M_assign(__in);
      return *this;
    }

    _GLIBCXX20_CONSTEXPR
    tuple& operator=(typename conditional<__assignable<_Elements...>(), tuple&&, __nonesuch&&>::type __in) noexcept(
        __nothrow_assignable<_Elements...>()) {
      this->_M_assign(std::move(__in));
      return *this;
    }

    template <typename... _UElements>
    _GLIBCXX20_CONSTEXPR __enable_if_t<__assignable<const _UElements&...>(), tuple&>
    operator=(const tuple<_UElements...>& __in) noexcept(__nothrow_assignable<const _UElements&...>()) {
      this->_M_assign(__in);
      return *this;
    }

    template <typename... _UElements>
    _GLIBCXX20_CONSTEXPR __enable_if_t<__assignable<_UElements...>(), tuple&>
    operator=(tuple<_UElements...>&& __in) noexcept(__nothrow_assignable<_UElements...>()) {
      this->_M_assign(std::move(__in));
      return *this;
    }

    // tuple swap
    _GLIBCXX20_CONSTEXPR
    void swap(tuple& __in) noexcept(__and_<__is_nothrow_swappable<_Elements>...>::value) { _Inherited::_M_swap(__in); }
  };

#if __cpp_deduction_guides >= 201606
  template <typename... _UTypes> tuple(_UTypes...) -> tuple<_UTypes...>;
  template <typename _T1, typename _T2> tuple(pair<_T1, _T2>) -> tuple<_T1, _T2>;
  template <typename _Alloc, typename... _UTypes> tuple(allocator_arg_t, _Alloc, _UTypes...) -> tuple<_UTypes...>;
  template <typename _Alloc, typename _T1, typename _T2>
  tuple(allocator_arg_t, _Alloc, pair<_T1, _T2>) -> tuple<_T1, _T2>;
  template <typename _Alloc, typename... _UTypes>
  tuple(allocator_arg_t, _Alloc, tuple<_UTypes...>) -> tuple<_UTypes...>;
#endif

  // Explicit specialization, zero-element tuple.
  template <> class tuple<> {
  public:
    void swap(tuple&) noexcept { /* no-op */
    }
    // We need the default since we're going to define no-op
    // allocator constructors.
    tuple() = default;
    // No-op allocator constructors.
    template <typename _Alloc> _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t, const _Alloc&) noexcept {}
    template <typename _Alloc> _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t, const _Alloc&, const tuple&) noexcept {}
  };

  /// Partial specialization, 2-element tuple.
  /// Includes construction and assignment from a pair.
  template <typename _T1, typename _T2> class tuple<_T1, _T2> : public _Tuple_impl<0, _T1, _T2> {
    typedef _Tuple_impl<0, _T1, _T2> _Inherited;

    // Constraint for non-explicit default constructor
    template <bool _Dummy, typename _U1, typename _U2>
    using _ImplicitDefaultCtor =
        __enable_if_t<_TupleConstraints<_Dummy, _U1, _U2>::__is_implicitly_default_constructible(), bool>;

    // Constraint for explicit default constructor
    template <bool _Dummy, typename _U1, typename _U2>
    using _ExplicitDefaultCtor =
        __enable_if_t<_TupleConstraints<_Dummy, _U1, _U2>::__is_explicitly_default_constructible(), bool>;

    template <bool _Dummy> using _TCC = _TupleConstraints<_Dummy, _T1, _T2>;

    // Constraint for non-explicit constructors
    template <bool _Cond, typename _U1, typename _U2>
    using _ImplicitCtor = __enable_if_t<_TCC<_Cond>::template __is_implicitly_constructible<_U1, _U2>(), bool>;

    // Constraint for non-explicit constructors
    template <bool _Cond, typename _U1, typename _U2>
    using _ExplicitCtor = __enable_if_t<_TCC<_Cond>::template __is_explicitly_constructible<_U1, _U2>(), bool>;

    template <typename _U1, typename _U2> static constexpr bool __assignable() {
      return __and_<is_assignable<_T1&, _U1>, is_assignable<_T2&, _U2>>::value;
    }

    template <typename _U1, typename _U2> static constexpr bool __nothrow_assignable() {
      return __and_<is_nothrow_assignable<_T1&, _U1>, is_nothrow_assignable<_T2&, _U2>>::value;
    }

    template <typename _U1, typename _U2> static constexpr bool __nothrow_constructible() {
      return __and_<is_nothrow_constructible<_T1, _U1>, is_nothrow_constructible<_T2, _U2>>::value;
    }

    static constexpr bool __nothrow_default_constructible() {
      return __and_<is_nothrow_default_constructible<_T1>, is_nothrow_default_constructible<_T2>>::value;
    }

    template <typename _U1> static constexpr bool __is_alloc_arg() {
      return is_same<__remove_cvref_t<_U1>, allocator_arg_t>::value;
    }

  public:
    template <bool _Dummy = true, _ImplicitDefaultCtor<_Dummy, _T1, _T2> = true>
    constexpr tuple() noexcept(__nothrow_default_constructible()) : _Inherited() {}

    template <bool _Dummy = true, _ExplicitDefaultCtor<_Dummy, _T1, _T2> = false>
    explicit constexpr tuple() noexcept(__nothrow_default_constructible()) : _Inherited() {}

    template <bool _Dummy = true, _ImplicitCtor<_Dummy, const _T1&, const _T2&> = true>
    constexpr tuple(const _T1& __a1, const _T2& __a2) noexcept(__nothrow_constructible<const _T1&, const _T2&>())
        : _Inherited(__a1, __a2) {}

    template <bool _Dummy = true, _ExplicitCtor<_Dummy, const _T1&, const _T2&> = false>
    explicit constexpr tuple(const _T1& __a1,
                             const _T2& __a2) noexcept(__nothrow_constructible<const _T1&, const _T2&>())
        : _Inherited(__a1, __a2) {}

    template <typename _U1, typename _U2, _ImplicitCtor<!__is_alloc_arg<_U1>(), _U1, _U2> = true>
    constexpr tuple(_U1&& __a1, _U2&& __a2) noexcept(__nothrow_constructible<_U1, _U2>())
        : _Inherited(std::forward<_U1>(__a1), std::forward<_U2>(__a2)) {}

    template <typename _U1, typename _U2, _ExplicitCtor<!__is_alloc_arg<_U1>(), _U1, _U2> = false>
    explicit constexpr tuple(_U1&& __a1, _U2&& __a2) noexcept(__nothrow_constructible<_U1, _U2>())
        : _Inherited(std::forward<_U1>(__a1), std::forward<_U2>(__a2)) {}

    constexpr tuple(const tuple&) = default;

    constexpr tuple(tuple&&) = default;

    template <typename _U1, typename _U2, _ImplicitCtor<true, const _U1&, const _U2&> = true>
    constexpr tuple(const tuple<_U1, _U2>& __in) noexcept(__nothrow_constructible<const _U1&, const _U2&>())
        : _Inherited(static_cast<const _Tuple_impl<0, _U1, _U2>&>(__in)) {}

    template <typename _U1, typename _U2, _ExplicitCtor<true, const _U1&, const _U2&> = false>
    explicit constexpr tuple(const tuple<_U1, _U2>& __in) noexcept(__nothrow_constructible<const _U1&, const _U2&>())
        : _Inherited(static_cast<const _Tuple_impl<0, _U1, _U2>&>(__in)) {}

    template <typename _U1, typename _U2, _ImplicitCtor<true, _U1, _U2> = true>
    constexpr tuple(tuple<_U1, _U2>&& __in) noexcept(__nothrow_constructible<_U1, _U2>())
        : _Inherited(static_cast<_Tuple_impl<0, _U1, _U2>&&>(__in)) {}

    template <typename _U1, typename _U2, _ExplicitCtor<true, _U1, _U2> = false>
    explicit constexpr tuple(tuple<_U1, _U2>&& __in) noexcept(__nothrow_constructible<_U1, _U2>())
        : _Inherited(static_cast<_Tuple_impl<0, _U1, _U2>&&>(__in)) {}

    template <typename _U1, typename _U2, _ImplicitCtor<true, const _U1&, const _U2&> = true>
    constexpr tuple(const pair<_U1, _U2>& __in) noexcept(__nothrow_constructible<const _U1&, const _U2&>())
        : _Inherited(__in.first, __in.second) {}

    template <typename _U1, typename _U2, _ExplicitCtor<true, const _U1&, const _U2&> = false>
    explicit constexpr tuple(const pair<_U1, _U2>& __in) noexcept(__nothrow_constructible<const _U1&, const _U2&>())
        : _Inherited(__in.first, __in.second) {}

    template <typename _U1, typename _U2, _ImplicitCtor<true, _U1, _U2> = true>
    constexpr tuple(pair<_U1, _U2>&& __in) noexcept(__nothrow_constructible<_U1, _U2>())
        : _Inherited(std::forward<_U1>(__in.first), std::forward<_U2>(__in.second)) {}

    template <typename _U1, typename _U2, _ExplicitCtor<true, _U1, _U2> = false>
    explicit constexpr tuple(pair<_U1, _U2>&& __in) noexcept(__nothrow_constructible<_U1, _U2>())
        : _Inherited(std::forward<_U1>(__in.first), std::forward<_U2>(__in.second)) {}

    // Allocator-extended constructors.

    template <typename _Alloc, _ImplicitDefaultCtor<is_object<_Alloc>::value, _T1, _T2> = true>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a) : _Inherited(__tag, __a) {}

    template <typename _Alloc, bool _Dummy = true, _ImplicitCtor<_Dummy, const _T1&, const _T2&> = true>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, const _T1& __a1, const _T2& __a2)
        : _Inherited(__tag, __a, __a1, __a2) {}

    template <typename _Alloc, bool _Dummy = true, _ExplicitCtor<_Dummy, const _T1&, const _T2&> = false>
    explicit _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, const _T1& __a1, const _T2& __a2)
        : _Inherited(__tag, __a, __a1, __a2) {}

    template <typename _Alloc, typename _U1, typename _U2, _ImplicitCtor<true, _U1, _U2> = true>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, _U1&& __a1, _U2&& __a2)
        : _Inherited(__tag, __a, std::forward<_U1>(__a1), std::forward<_U2>(__a2)) {}

    template <typename _Alloc, typename _U1, typename _U2, _ExplicitCtor<true, _U1, _U2> = false>
    explicit _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, _U1&& __a1, _U2&& __a2)
        : _Inherited(__tag, __a, std::forward<_U1>(__a1), std::forward<_U2>(__a2)) {}

    template <typename _Alloc>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, const tuple& __in)
        : _Inherited(__tag, __a, static_cast<const _Inherited&>(__in)) {}

    template <typename _Alloc>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, tuple&& __in)
        : _Inherited(__tag, __a, static_cast<_Inherited&&>(__in)) {}

    template <typename _Alloc, typename _U1, typename _U2, _ImplicitCtor<true, const _U1&, const _U2&> = true>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, const tuple<_U1, _U2>& __in)
        : _Inherited(__tag, __a, static_cast<const _Tuple_impl<0, _U1, _U2>&>(__in)) {}

    template <typename _Alloc, typename _U1, typename _U2, _ExplicitCtor<true, const _U1&, const _U2&> = false>
    explicit _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, const tuple<_U1, _U2>& __in)
        : _Inherited(__tag, __a, static_cast<const _Tuple_impl<0, _U1, _U2>&>(__in)) {}

    template <typename _Alloc, typename _U1, typename _U2, _ImplicitCtor<true, _U1, _U2> = true>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, tuple<_U1, _U2>&& __in)
        : _Inherited(__tag, __a, static_cast<_Tuple_impl<0, _U1, _U2>&&>(__in)) {}

    template <typename _Alloc, typename _U1, typename _U2, _ExplicitCtor<true, _U1, _U2> = false>
    explicit _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, tuple<_U1, _U2>&& __in)
        : _Inherited(__tag, __a, static_cast<_Tuple_impl<0, _U1, _U2>&&>(__in)) {}

    template <typename _Alloc, typename _U1, typename _U2, _ImplicitCtor<true, const _U1&, const _U2&> = true>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, const pair<_U1, _U2>& __in)
        : _Inherited(__tag, __a, __in.first, __in.second) {}

    template <typename _Alloc, typename _U1, typename _U2, _ExplicitCtor<true, const _U1&, const _U2&> = false>
    explicit _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, const pair<_U1, _U2>& __in)
        : _Inherited(__tag, __a, __in.first, __in.second) {}

    template <typename _Alloc, typename _U1, typename _U2, _ImplicitCtor<true, _U1, _U2> = true>
    _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, pair<_U1, _U2>&& __in)
        : _Inherited(__tag, __a, std::forward<_U1>(__in.first), std::forward<_U2>(__in.second)) {}

    template <typename _Alloc, typename _U1, typename _U2, _ExplicitCtor<true, _U1, _U2> = false>
    explicit _GLIBCXX20_CONSTEXPR tuple(allocator_arg_t __tag, const _Alloc& __a, pair<_U1, _U2>&& __in)
        : _Inherited(__tag, __a, std::forward<_U1>(__in.first), std::forward<_U2>(__in.second)) {}

    // Tuple assignment.

    _GLIBCXX20_CONSTEXPR
    tuple& operator=(typename conditional<__assignable<const _T1&, const _T2&>(), const tuple&, const __nonesuch&>::type
                         __in) noexcept(__nothrow_assignable<const _T1&, const _T2&>()) {
      this->_M_assign(__in);
      return *this;
    }

    _GLIBCXX20_CONSTEXPR
    tuple& operator=(typename conditional<__assignable<_T1, _T2>(), tuple&&, __nonesuch&&>::type __in) noexcept(
        __nothrow_assignable<_T1, _T2>()) {
      this->_M_assign(std::move(__in));
      return *this;
    }

    template <typename _U1, typename _U2>
    _GLIBCXX20_CONSTEXPR __enable_if_t<__assignable<const _U1&, const _U2&>(), tuple&>
    operator=(const tuple<_U1, _U2>& __in) noexcept(__nothrow_assignable<const _U1&, const _U2&>()) {
      this->_M_assign(__in);
      return *this;
    }

    template <typename _U1, typename _U2>
    _GLIBCXX20_CONSTEXPR __enable_if_t<__assignable<_U1, _U2>(), tuple&>
    operator=(tuple<_U1, _U2>&& __in) noexcept(__nothrow_assignable<_U1, _U2>()) {
      this->_M_assign(std::move(__in));
      return *this;
    }

    template <typename _U1, typename _U2>
    _GLIBCXX20_CONSTEXPR __enable_if_t<__assignable<const _U1&, const _U2&>(), tuple&>
    operator=(const pair<_U1, _U2>& __in) noexcept(__nothrow_assignable<const _U1&, const _U2&>()) {
      this->_M_head(*this)                = __in.first;
      this->_M_tail(*this)._M_head(*this) = __in.second;
      return *this;
    }

    template <typename _U1, typename _U2>
    _GLIBCXX20_CONSTEXPR __enable_if_t<__assignable<_U1, _U2>(), tuple&>
    operator=(pair<_U1, _U2>&& __in) noexcept(__nothrow_assignable<_U1, _U2>()) {
      this->_M_head(*this)                = std::forward<_U1>(__in.first);
      this->_M_tail(*this)._M_head(*this) = std::forward<_U2>(__in.second);
      return *this;
    }

    _GLIBCXX20_CONSTEXPR
    void swap(tuple& __in) noexcept(__and_<__is_nothrow_swappable<_T1>, __is_nothrow_swappable<_T2>>::value) {
      _Inherited::_M_swap(__in);
    }
  };

  /// class tuple_size
  template <typename... _Elements>
  struct tuple_size<tuple<_Elements...>> : public integral_constant<std::size_t, sizeof...(_Elements)> {};

#if __cplusplus > 201402L
  template <typename _Tp> inline constexpr size_t tuple_size_v = tuple_size<_Tp>::value;
#endif

  /**
   * Recursive case for tuple_element: strip off the first element in
   * the tuple and retrieve the (i-1)th element of the remaining tuple.
   */
  template <std::size_t __i, typename _Head, typename... _Tail>
  struct tuple_element<__i, tuple<_Head, _Tail...>> : tuple_element<__i - 1, tuple<_Tail...>> {};

  /**
   * Basis case for tuple_element: The first element is the one we're seeking.
   */
  template <typename _Head, typename... _Tail> struct tuple_element<0, tuple<_Head, _Tail...>> { typedef _Head type; };

  /**
   * Error case for tuple_element: invalid index.
   */
  template <size_t __i> struct tuple_element<__i, tuple<>> {
    static_assert(__i < tuple_size<tuple<>>::value, "tuple index is in range");
  };

  template <std::size_t __i, typename _Head, typename... _Tail>
  constexpr _Head& __get_helper(_Tuple_impl<__i, _Head, _Tail...> & __t) noexcept {
    return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t);
  }

  template <std::size_t __i, typename _Head, typename... _Tail>
  constexpr const _Head& __get_helper(const _Tuple_impl<__i, _Head, _Tail...>& __t) noexcept {
    return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t);
  }

  /// Return a reference to the ith element of a tuple.
  template <std::size_t __i, typename... _Elements>
  constexpr __tuple_element_t<__i, tuple<_Elements...>>& get(tuple<_Elements...> & __t) noexcept {
    return std::__get_helper<__i>(__t);
  }

  /// Return a const reference to the ith element of a const tuple.
  template <std::size_t __i, typename... _Elements>
  constexpr const __tuple_element_t<__i, tuple<_Elements...>>& get(const tuple<_Elements...>& __t) noexcept {
    return std::__get_helper<__i>(__t);
  }

  /// Return an rvalue reference to the ith element of a tuple rvalue.
  template <std::size_t __i, typename... _Elements>
  constexpr __tuple_element_t<__i, tuple<_Elements...>>&& get(tuple<_Elements...> && __t) noexcept {
    typedef __tuple_element_t<__i, tuple<_Elements...>> __element_type;
    return std::forward<__element_type&&>(std::get<__i>(__t));
  }

  /// Return a const rvalue reference to the ith element of a const tuple rvalue.
  template <std::size_t __i, typename... _Elements>
  constexpr const __tuple_element_t<__i, tuple<_Elements...>>&& get(const tuple<_Elements...>&& __t) noexcept {
    typedef __tuple_element_t<__i, tuple<_Elements...>> __element_type;
    return std::forward<const __element_type&&>(std::get<__i>(__t));
  }

#if __cplusplus >= 201402L

#define __cpp_lib_tuples_by_type 201304

  template <typename _Head, size_t __i, typename... _Tail>
  constexpr _Head& __get_helper2(_Tuple_impl<__i, _Head, _Tail...> & __t) noexcept {
    return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t);
  }

  template <typename _Head, size_t __i, typename... _Tail>
  constexpr const _Head& __get_helper2(const _Tuple_impl<__i, _Head, _Tail...>& __t) noexcept {
    return _Tuple_impl<__i, _Head, _Tail...>::_M_head(__t);
  }

  /// Return a reference to the unique element of type _Tp of a tuple.
  template <typename _Tp, typename... _Types> constexpr _Tp& get(tuple<_Types...> & __t) noexcept {
    return std::__get_helper2<_Tp>(__t);
  }

  /// Return a reference to the unique element of type _Tp of a tuple rvalue.
  template <typename _Tp, typename... _Types> constexpr _Tp&& get(tuple<_Types...> && __t) noexcept {
    return std::forward<_Tp&&>(std::__get_helper2<_Tp>(__t));
  }

  /// Return a const reference to the unique element of type _Tp of a tuple.
  template <typename _Tp, typename... _Types> constexpr const _Tp& get(const tuple<_Types...>& __t) noexcept {
    return std::__get_helper2<_Tp>(__t);
  }

  /// Return a const reference to the unique element of type _Tp of
  /// a const tuple rvalue.
  template <typename _Tp, typename... _Types> constexpr const _Tp&& get(const tuple<_Types...>&& __t) noexcept {
    return std::forward<const _Tp&&>(std::__get_helper2<_Tp>(__t));
  }
#endif

  // This class performs the comparison operations on tuples
  template <typename _Tp, typename _Up, size_t __i, size_t __size> struct __tuple_compare {
    static constexpr bool __eq(const _Tp& __t, const _Up& __u) {
      return bool(std::get<__i>(__t) == std::get<__i>(__u)) &&
             __tuple_compare<_Tp, _Up, __i + 1, __size>::__eq(__t, __u);
    }

    static constexpr bool __less(const _Tp& __t, const _Up& __u) {
      return bool(std::get<__i>(__t) < std::get<__i>(__u)) ||
             (!bool(std::get<__i>(__u) < std::get<__i>(__t)) &&
              __tuple_compare<_Tp, _Up, __i + 1, __size>::__less(__t, __u));
    }
  };

  template <typename _Tp, typename _Up, size_t __size> struct __tuple_compare<_Tp, _Up, __size, __size> {
    static constexpr bool __eq(const _Tp&, const _Up&) { return true; }

    static constexpr bool __less(const _Tp&, const _Up&) { return false; }
  };

  template <typename... _TElements, typename... _UElements>
  constexpr bool operator==(const tuple<_TElements...>& __t, const tuple<_UElements...>& __u) {
    static_assert(sizeof...(_TElements) == sizeof...(_UElements),
                  "tuple objects can only be compared if they have equal sizes.");
    using __compare = __tuple_compare<tuple<_TElements...>, tuple<_UElements...>, 0, sizeof...(_TElements)>;
    return __compare::__eq(__t, __u);
  }

#if __cpp_lib_three_way_comparison
  template <typename _Cat, typename _Tp, typename _Up>
  constexpr _Cat __tuple_cmp(const _Tp&, const _Up&, index_sequence<>) {
    return _Cat::equivalent;
  }

  template <typename _Cat, typename _Tp, typename _Up, size_t _Idx0, size_t... _Idxs>
  constexpr _Cat __tuple_cmp(const _Tp& __t, const _Up& __u, index_sequence<_Idx0, _Idxs...>) {
    auto __c = __detail::__synth3way(std::get<_Idx0>(__t), std::get<_Idx0>(__u));
    if (__c != 0)
      return __c;
    return std::__tuple_cmp<_Cat>(__t, __u, index_sequence<_Idxs...>());
  }

  template <typename... _Tps, typename... _Ups>
  constexpr common_comparison_category_t<__detail::__synth3way_t<_Tps, _Ups>...> operator<=>(
      const tuple<_Tps...>& __t, const tuple<_Ups...>& __u) {
    using _Cat = common_comparison_category_t<__detail::__synth3way_t<_Tps, _Ups>...>;
    return std::__tuple_cmp<_Cat>(__t, __u, index_sequence_for<_Tps...>());
  }
#else
  template <typename... _TElements, typename... _UElements>
  constexpr bool operator<(const tuple<_TElements...>& __t, const tuple<_UElements...>& __u) {
    static_assert(sizeof...(_TElements) == sizeof...(_UElements),
                  "tuple objects can only be compared if they have equal sizes.");
    using __compare = __tuple_compare<tuple<_TElements...>, tuple<_UElements...>, 0, sizeof...(_TElements)>;
    return __compare::__less(__t, __u);
  }

  template <typename... _TElements, typename... _UElements>
  constexpr bool operator!=(const tuple<_TElements...>& __t, const tuple<_UElements...>& __u) {
    return !(__t == __u);
  }

  template <typename... _TElements, typename... _UElements>
  constexpr bool operator>(const tuple<_TElements...>& __t, const tuple<_UElements...>& __u) {
    return __u < __t;
  }

  template <typename... _TElements, typename... _UElements>
  constexpr bool operator<=(const tuple<_TElements...>& __t, const tuple<_UElements...>& __u) {
    return !(__u < __t);
  }

  template <typename... _TElements, typename... _UElements>
  constexpr bool operator>=(const tuple<_TElements...>& __t, const tuple<_UElements...>& __u) {
    return !(__t < __u);
  }
#endif                                                 // three_way_comparison

  // NB: DR 705.
  template <typename... _Elements>
  constexpr tuple<typename __decay_and_strip<_Elements>::__type...> make_tuple(_Elements && ... __args) {
    typedef tuple<typename __decay_and_strip<_Elements>::__type...> __result_type;
    return __result_type(std::forward<_Elements>(__args)...);
  }

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 2275. Why is forward_as_tuple not constexpr?
  /// std::forward_as_tuple
  template <typename... _Elements> constexpr tuple<_Elements&&...> forward_as_tuple(_Elements && ... __args) noexcept {
    return tuple<_Elements&&...>(std::forward<_Elements>(__args)...);
  }

  template <size_t, typename, typename, size_t> struct __make_tuple_impl;

  template <size_t _Idx, typename _Tuple, typename... _Tp, size_t _Nm>
  struct __make_tuple_impl<_Idx, tuple<_Tp...>, _Tuple, _Nm>
      : __make_tuple_impl<_Idx + 1, tuple<_Tp..., __tuple_element_t<_Idx, _Tuple>>, _Tuple, _Nm> {};

  template <std::size_t _Nm, typename _Tuple, typename... _Tp>
  struct __make_tuple_impl<_Nm, tuple<_Tp...>, _Tuple, _Nm> {
    typedef tuple<_Tp...> __type;
  };

  template <typename _Tuple>
  struct __do_make_tuple : __make_tuple_impl<0, tuple<>, _Tuple, std::tuple_size<_Tuple>::value> {};

  // Returns the std::tuple equivalent of a tuple-like type.
  template <typename _Tuple> struct __make_tuple : public __do_make_tuple<__remove_cvref_t<_Tuple>> {};

  // Combines several std::tuple's into a single one.
  template <typename...> struct __combine_tuples;

  template <> struct __combine_tuples<> { typedef tuple<> __type; };

  template <typename... _Ts> struct __combine_tuples<tuple<_Ts...>> { typedef tuple<_Ts...> __type; };

  template <typename... _T1s, typename... _T2s, typename... _Rem>
  struct __combine_tuples<tuple<_T1s...>, tuple<_T2s...>, _Rem...> {
    typedef typename __combine_tuples<tuple<_T1s..., _T2s...>, _Rem...>::__type __type;
  };

  // Computes the result type of tuple_cat given a set of tuple-like types.
  template <typename... _Tpls> struct __tuple_cat_result {
    typedef typename __combine_tuples<typename __make_tuple<_Tpls>::__type...>::__type __type;
  };

  // Helper to determine the index set for the first tuple-like
  // type of a given set.
  template <typename...> struct __make_1st_indices;

  template <> struct __make_1st_indices<> { typedef std::_Index_tuple<> __type; };

  template <typename _Tp, typename... _Tpls> struct __make_1st_indices<_Tp, _Tpls...> {
    typedef typename std::_Build_index_tuple<std::tuple_size<typename std::remove_reference<_Tp>::type>::value>::__type
        __type;
  };

  // Performs the actual concatenation by step-wise expanding tuple-like
  // objects into the elements,  which are finally forwarded into the
  // result tuple.
  template <typename _Ret, typename _Indices, typename... _Tpls> struct __tuple_concater;

  template <typename _Ret, std::size_t... _Is, typename _Tp, typename... _Tpls>
  struct __tuple_concater<_Ret, std::_Index_tuple<_Is...>, _Tp, _Tpls...> {
    template <typename... _Us> static constexpr _Ret _S_do(_Tp&& __tp, _Tpls&&... __tps, _Us&&... __us) {
      typedef typename __make_1st_indices<_Tpls...>::__type __idx;
      typedef __tuple_concater<_Ret, __idx, _Tpls...> __next;
      return __next::_S_do(std::forward<_Tpls>(__tps)..., std::forward<_Us>(__us)...,
                           std::get<_Is>(std::forward<_Tp>(__tp))...);
    }
  };

  template <typename _Ret> struct __tuple_concater<_Ret, std::_Index_tuple<>> {
    template <typename... _Us> static constexpr _Ret _S_do(_Us&&... __us) { return _Ret(std::forward<_Us>(__us)...); }
  };

  /// tuple_cat
  template <typename... _Tpls, typename = typename enable_if<__and_<__is_tuple_like<_Tpls>...>::value>::type>
  constexpr auto tuple_cat(_Tpls && ... __tpls)->typename __tuple_cat_result<_Tpls...>::__type {
    typedef typename __tuple_cat_result<_Tpls...>::__type __ret;
    typedef typename __make_1st_indices<_Tpls...>::__type __idx;
    typedef __tuple_concater<__ret, __idx, _Tpls...> __concater;
    return __concater::_S_do(std::forward<_Tpls>(__tpls)...);
  }

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 2301. Why is tie not constexpr?
  /// tie
  template <typename... _Elements> constexpr tuple<_Elements&...> tie(_Elements & ... __args) noexcept {
    return tuple<_Elements&...>(__args...);
  }

  /// swap
  template <typename... _Elements>
  _GLIBCXX20_CONSTEXPR inline
#if __cplusplus > 201402L || !defined(__STRICT_ANSI__) // c++1z or gnu++11
      // Constrained free swap overload, see p0185r1
      typename enable_if<__and_<__is_swappable<_Elements>...>::value>::type
#else
      void
#endif
      swap(tuple<_Elements...> & __x, tuple<_Elements...> & __y) noexcept(noexcept(__x.swap(__y))) {
    __x.swap(__y);
  }

#if __cplusplus > 201402L || !defined(__STRICT_ANSI__) // c++1z or gnu++11
  template <typename... _Elements>
  _GLIBCXX20_CONSTEXPR typename enable_if<!__and_<__is_swappable<_Elements>...>::value>::type swap(
      tuple<_Elements...>&, tuple<_Elements...>&) = delete;
#endif

  // A class (and instance) which can be used in 'tie' when an element
  // of a tuple is not required.
  // _GLIBCXX14_CONSTEXPR
  // 2933. PR for LWG 2773 could be clearer
  struct _Swallow_assign {
    template <class _Tp> _GLIBCXX14_CONSTEXPR const _Swallow_assign& operator=(const _Tp&) const { return *this; }
  };

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 2773. Making std::ignore constexpr
  _GLIBCXX17_INLINE constexpr _Swallow_assign ignore{};

  /// Partial specialization for tuples
  template <typename... _Types, typename _Alloc> struct uses_allocator<tuple<_Types...>, _Alloc> : true_type {};

  // See stl_pair.h...
  /** "piecewise construction" using a tuple of arguments for each member.
   *
   * @param __first Arguments for the first member of the pair.
   * @param __second Arguments for the second member of the pair.
   *
   * The elements of each tuple will be used as the constructor arguments
   * for the data members of the pair.
   */
  template <class _T1, class _T2>
  template <typename... _Args1, typename... _Args2>
  _GLIBCXX20_CONSTEXPR inline pair<_T1, _T2>::pair(piecewise_construct_t, tuple<_Args1...> __first,
                                                   tuple<_Args2...> __second)
      : pair(__first, __second, typename _Build_index_tuple<sizeof...(_Args1)>::__type(),
             typename _Build_index_tuple<sizeof...(_Args2)>::__type()) {}

  template <class _T1, class _T2>
  template <typename... _Args1, std::size_t... _Indexes1, typename... _Args2, std::size_t... _Indexes2>
  _GLIBCXX20_CONSTEXPR inline pair<_T1, _T2>::pair(tuple<_Args1...> & __tuple1, tuple<_Args2...> & __tuple2,
                                                   _Index_tuple<_Indexes1...>, _Index_tuple<_Indexes2...>)
      : first(std::forward<_Args1>(std::get<_Indexes1>(__tuple1))...),
        second(std::forward<_Args2>(std::get<_Indexes2>(__tuple2))...) {}

#if __cplusplus >= 201703L

  // Unpack a std::tuple into a type trait and use its value.
  // For cv std::tuple<_Up> the result is _Trait<_Tp, cv _Up...>::value.
  // For cv std::tuple<_Up>& the result is _Trait<_Tp, cv _Up&...>::value.
  // Otherwise the result is false (because we don't know if std::get throws).
  template <template <typename...> class _Trait, typename _Tp, typename _Tuple>
  inline constexpr bool __unpack_std_tuple = false;

  template <template <typename...> class _Trait, typename _Tp, typename... _Up>
  inline constexpr bool __unpack_std_tuple<_Trait, _Tp, tuple<_Up...>> = _Trait<_Tp, _Up...>::value;

  template <template <typename...> class _Trait, typename _Tp, typename... _Up>
  inline constexpr bool __unpack_std_tuple<_Trait, _Tp, tuple<_Up...>&> = _Trait<_Tp, _Up&...>::value;

  template <template <typename...> class _Trait, typename _Tp, typename... _Up>
  inline constexpr bool __unpack_std_tuple<_Trait, _Tp, const tuple<_Up...>> = _Trait<_Tp, const _Up...>::value;

  template <template <typename...> class _Trait, typename _Tp, typename... _Up>
  inline constexpr bool __unpack_std_tuple<_Trait, _Tp, const tuple<_Up...>&> = _Trait<_Tp, const _Up&...>::value;

#define __cpp_lib_apply 201603

  template <typename _Fn, typename _Tuple, size_t... _Idx>
  constexpr decltype(auto) __apply_impl(_Fn && __f, _Tuple && __t, index_sequence<_Idx...>) {
    return std::__invoke(std::forward<_Fn>(__f), std::get<_Idx>(std::forward<_Tuple>(__t))...);
  }

  template <typename _Fn, typename _Tuple>
  constexpr decltype(auto) apply(_Fn && __f,
                                 _Tuple && __t) noexcept(__unpack_std_tuple<is_nothrow_invocable, _Fn, _Tuple>) {
    using _Indices = make_index_sequence<tuple_size_v<remove_reference_t<_Tuple>>>;
    return std::__apply_impl(std::forward<_Fn>(__f), std::forward<_Tuple>(__t), _Indices{});
  }

#define __cpp_lib_make_from_tuple 201606

  template <typename _Tp, typename _Tuple, size_t... _Idx>
  constexpr _Tp __make_from_tuple_impl(_Tuple && __t, index_sequence<_Idx...>) {
    return _Tp(std::get<_Idx>(std::forward<_Tuple>(__t))...);
  }

  template <typename _Tp, typename _Tuple>
  constexpr _Tp make_from_tuple(_Tuple && __t) noexcept(__unpack_std_tuple<is_nothrow_constructible, _Tp, _Tuple>) {
    return __make_from_tuple_impl<_Tp>(std::forward<_Tuple>(__t),
                                       make_index_sequence<tuple_size_v<remove_reference_t<_Tuple>>>{});
  }
#endif // C++17

  /// @}

  _GLIBCXX_END_NAMESPACE_VERSION
} // namespace )

#endif // C++11

#endif // _GLIBCXX_TUPLE