#pragma once

#ifndef extern_C
#define extern_C extern "C"
#endif

#ifndef static_test
#define static_test static_assert
#endif

#ifndef noncopyable
#define noncopyable(name)                                                                                              \
  name(const name&)            = delete; /* NOLINT(bugprone-macro-parentheses) */                                      \
  name& operator=(const name&) = delete  /* NOLINT(bugprone-macro-parentheses) */
#endif

#ifndef nonmovable
#define nonmovable(name)                                                                                               \
  name(name&&)            = delete; /* NOLINT(bugprone-macro-parentheses) */                                           \
  name& operator=(name&&) = delete  /* NOLINT(bugprone-macro-parentheses) */
#endif
