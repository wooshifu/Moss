#pragma once

#include "concepts"
#include "limits"

enum class KErrorCode : int {
  OK = 0,

  UNKNOWN = std::numeric_limits<int>::min(),
};

inline bool ok(KErrorCode code) { return code == KErrorCode::OK; }
inline bool not_ok(KErrorCode code) { return not ok(code); }

inline bool ok(bool code) { return code; }
inline bool not_ok(bool code) { return not ok(code); }

/// convention: zero represents ok, non-zero represents not ok
template <typename T>
concept Number = std::integral<T> && not std::same_as<T, bool>;
inline bool ok(Number auto code) { return code == 0; }
inline bool not_ok(Number auto code) { return not ok(code); }
