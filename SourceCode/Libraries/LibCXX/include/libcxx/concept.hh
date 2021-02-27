#pragma once

#include "libcxx/std.hh"
#include "libcxx/type_traits.hh"

NAMESPACE_STD_BEGIN

/// [concept.derived], concept derived_from
template <typename _Derived, typename _Base>
concept derived_from =
    __is_base_of(_Base, _Derived) && is_convertible_v<const volatile _Derived*, const volatile _Base*>;

NAMESPACE_STD_END
