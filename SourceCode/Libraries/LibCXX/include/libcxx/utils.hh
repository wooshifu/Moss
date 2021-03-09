#pragma once

#include "concepts"
#include "type_traits"

template <typename EnumClass>
requires std::is_enum_v<EnumClass> inline auto underlying_value(const EnumClass value) ->
    typename std::underlying_type_t<EnumClass> {
  return static_cast<typename std::underlying_type_t<EnumClass>>(value);
}
