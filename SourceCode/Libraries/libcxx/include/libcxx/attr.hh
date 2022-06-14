#pragma once

#ifndef ATTR_NO_SAFESTACK
#define ATTR_NO_SAFESTACK [[clang::__no_sanitize__("safe-stack", "shadow-call-stack")]]
#endif

#ifndef ATTR_NO_ASAN
#define ATTR_NO_ASAN [[clang::no_sanitize("address")]]
#endif

#ifndef ATTR_ALIGNED
#define ATTR_ALIGNED(align) [[gnu::aligned(align)]]
#endif

#ifndef ATTR_USED
#define ATTR_USED [[gnu::used]]
#endif

#ifndef ATTR_OPTNONE
#define ATTR_OPTNONE [[clang::optnone]]
#endif

#ifndef ATTR_NAKED
#define ATTR_NAKED [[gnu::naked]]
#endif

#ifndef ATTR_NORETURN
#define ATTR_NORETURN [[noreturn]]
#endif

#ifndef ATTR_FALLTHROUGH
#define ATTR_FALLTHROUGH [[fallthrough]]
#endif
