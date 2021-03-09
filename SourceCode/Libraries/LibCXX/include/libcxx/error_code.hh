#pragma once

#include "limits" // for numeric_limits

enum class KErrorCode : int {
  OK = 0,

  UNKNOWN = std::numeric_limits<int>::min(),
};

template <typename T> bool ok(T code);
template <typename T> bool not_ok(T code);
