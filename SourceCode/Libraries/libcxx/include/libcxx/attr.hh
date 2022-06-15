#pragma once

#ifndef attr_no_safestack
#define attr_no_safestack [[clang::__no_sanitize__("safe-stack", "shadow-call-stack")]]
#endif

#ifndef attr_no_asan
#define attr_no_asan [[clang::no_sanitize("address")]]
#endif

#ifndef attr_aligned
#define attr_aligned(align) [[gnu::aligned(align)]]
#endif

#ifndef attr_used
#define attr_used [[gnu::used]]
#endif

#ifndef attr_optnone
#define attr_optnone [[clang::optnone]]
#endif

#ifndef attr_naked
#define attr_naked [[gnu::naked]]
#endif

#ifndef attr_noreturn
#define attr_noreturn [[noreturn]]
#endif

#ifndef attr_fallthrough
#define attr_fallthrough [[fallthrough]]
#endif
