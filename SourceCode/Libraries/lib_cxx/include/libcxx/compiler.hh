#pragma once

#ifdef offsetof
#error "macro offsetof should not be override"
#endif

#define offsetof(type, member) __builtin_offsetof(type, member)
