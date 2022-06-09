#pragma once

// clang-format off

.macro FUNCTION x
  .global \x;
  .type \x,STT_FUNC;
    \x:
.endm

.macro LOCAL_FUNCTION x
  .type \x,STT_FUNC;
    \x:
.endm

.macro END_FUNCTION x
  .size \x, . - \x
.endm

.macro DATA x
  .global \x;
  .type \x,STT_OBJECT;
    \x:
.endm

.macro LOCAL_DATA x
  .type \x,STT_OBJECT;
    \x:
.endm

.macro END_DATA x
  .size \x, . - \x
.endm
