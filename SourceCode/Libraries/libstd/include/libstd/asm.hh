#pragma once

.macro LOCAL_DATA x
    .type \x,STT_OBJECT; \x:
.endm

.macro END_DATA x
    .size \x, . - \x
.endm

.macro DATA x
    .global \x; .type \x,STT_OBJECT; \x:
.endm
