#pragma once

//#define FUNCTION(x) .global x; .type x,STT_FUNC; x:

//.macro FUNCTION x
//    .global \x; .type \x,STT_FUNC; \x:
//.endm
//
//.macro END_FUNCTION x
//        .size \x, . - \x
//.endm

.if 0
#define DATA(x) .global x; .type x,STT_OBJECT; x:

#define LOCAL_FUNCTION(x) .type x,STT_FUNC; x:
#define LOCAL_DATA(x) .type x,STT_OBJECT; x:

#define END_FUNCTION(x) .size x, . - x
#define END_DATA(x) .size x, . - x
.endif

.macro LOCAL_DATA x
    .type \x,STT_OBJECT; \x:
.endm

.macro END_DATA x
    .size \x, . - \x
.endm

.macro DATA x
    .global \x; .type \x,STT_OBJECT; \x:
.endm