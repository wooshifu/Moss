#pragma once

// clang-format off
// The kernel is compiled using -ffixed-x20 so the compiler will never use this register.
percpu_ptr .req x20

// This register is permanently reserved by the ABI in the compiler.
// #if __has_feature(shadow_call_stack) it's used for the SCSP.
shadow_call_sp .req x18
