// Copyright 2020 The Fuchsia Authors
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

// NOTE: This is an independent header so it can be #include'd by a userland
// test program as well as the kernel source (both C++ and assembly).

#ifdef KERNEL_ASPACE_BASE
#error "KERNEL_ASPACE_BASE should not be overwrite"
#endif
#ifdef KERNEL_ASPACE_SIZE
#error "KERNEL_ASPACE_SIZE should not be overwrite"
#endif
// Virtual address where the kernel address space begins.
// Below this is the user address space.
#define KERNEL_ASPACE_BASE 0xffff000000000000UL
#ifndef __ASSEMBLER__
static_assert(KERNEL_ASPACE_BASE == 0xffff'0000'0000'0000UL);
#endif
#define KERNEL_ASPACE_SIZE 0x0001000000000000UL
#ifndef __ASSEMBLER__
static_assert(KERNEL_ASPACE_SIZE == 0x0001'0000'0000'0000UL);
#endif

#ifdef USER_ASPACE_BASE
#error "USER_ASPACE_BASE should not be overwrite"
#endif
#ifdef USER_ASPACE_SIZE
#error "USER_ASPACE_SIZE should not be overwrite"
#endif
// Virtual address where the user-accessible address space begins.
// Below this is wholly inaccessible.
#define USER_ASPACE_BASE 0x0000000001000000UL
#ifndef __ASSEMBLER__
static_assert(USER_ASPACE_BASE == 0x0000'0000'0100'0000UL);
#endif
#define USER_ASPACE_SIZE (0xffffff000000UL - USER_ASPACE_BASE)
#ifndef __ASSEMBLER__
static_assert(USER_ASPACE_SIZE == 0x0000'ffff'fe00'0000);
#endif
