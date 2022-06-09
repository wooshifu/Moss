#pragma once

#ifndef offsetof
// Get the offset of `field` from the beginning of the struct or class `type`.
#define offsetof(type, field) __builtin_offsetof(type, field)
#endif

// NOTE: don't impl this function, no impl will produce consteval failure
void trigger_consteval_failure(char const*);
