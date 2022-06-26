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

#ifndef attr_unused
#define attr_unused [[gnu::unused]]
#endif

#ifndef attr_maybe_unused
#define attr_maybe_unused [[maybe_unused]]
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

#ifndef attr_always_inline
#define attr_always_inline [[gnu::always_inline]]
#endif

#ifndef attr_likely
#define attr_likely [[likely]]
#endif

#ifndef attr_unlikely
#define attr_unlikely [[unlikely]]
#endif

#ifndef attr_printf
#define attr_printf(fmt, varargs) [[gnu::format(__printf__, fmt, varargs)]]
#endif

#ifndef attr_noinline
#define attr_noinline [[gnu::noinline]]
#endif