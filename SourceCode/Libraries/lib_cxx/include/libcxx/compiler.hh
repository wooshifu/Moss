//
// Created by shifu on 2021/10/17.
//

#pragma once

#ifndef __offsetof
// Get the offset of `field` from the beginning of the struct or class `type`.
#define __offsetof(type, field) __builtin_offsetof(type, field)
#endif

#ifndef offsetof
#define offsetof(type, field) __offsetof(type, field)
#endif
