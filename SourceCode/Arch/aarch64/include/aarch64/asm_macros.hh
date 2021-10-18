//
// Created by shifu on 2021/10/17.
//

#pragma once

// clang-format off

.macro movlit reg, literal
  mov \reg, #((\literal) & 0xffff)
.ifne (((\literal) >> 16) & 0xffff)
  movk \reg, #(((\literal) >> 16) & 0xffff), lsl #16
.endif
.ifne (((\literal) >> 32) & 0xffff)
  movk \reg, #(((\literal) >> 32) & 0xffff), lsl #32
.endif
.ifne (((\literal) >> 48) & 0xffff)
  movk \reg, #(((\literal) >> 48) & 0xffff), lsl #48
.endif
.endm

.macro adr_global reg, symbol
  adrp \reg, \symbol
  add \reg, \reg, #:lo12:\symbol
.endm


// for functions that don't have an "end" or don't want .cfi_startproc
.macro LOCAL_FUNCTION_LABEL x
  .type \x,STT_FUNC; \x:
.endm

.macro FUNCTION_LABEL x
  .global \x; .hidden \x; LOCAL_FUNCTION_LABEL \x
.endm

.macro LOCAL_FUNCTION x
  LOCAL_FUNCTION_LABEL \x
  .cfi_startproc
.endm

.macro FUNCTION x
  .global \x; .hidden \x; LOCAL_FUNCTION \x
.endm

// for local or global functions
.macro END_FUNCTION x
  .cfi_endproc; .size \x, . - \x
.endm

.macro LOCAL_DATA x
.type \x,STT_OBJECT; \x:
.endm

.macro DATA x
  .global \x; .hidden \x; LOCAL_DATA \x
.endm

// for local or global data
.macro END_DATA x
  .size \x, . - \x
.endm

        // clang-format on
