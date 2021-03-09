#include "libcxx/error_code.hh"

template <> bool ok(KErrorCode code) { return code == KErrorCode::OK; }
template <> bool not_ok(KErrorCode code) { return code != KErrorCode::OK; }

template <> bool ok(bool code) { return code; }
template <> bool not_ok(bool code) { return code; }

template <> bool ok(int code) { return code; }
template <> bool not_ok(int code) { return code; }
