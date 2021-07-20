#pragma once

using size_t    = __SIZE_TYPE__;
using ptrdiff_t = __PTRDIFF_TYPE__;

#if __cplusplus >= 201103L
using nullptr_t = decltype(nullptr);
#endif
