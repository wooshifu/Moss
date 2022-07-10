#pragma once

// clang-format off

.macro adr_g reg, symbol, hwaddress_sanitizer_enabled=0
  .if \hwaddress_sanitizer_enabled != 0
    adrp \reg, :pg_hi21_nc:\symbol
    movk \reg, #:prel_g3:\symbol+0x100000000
    add \reg, \reg, #:lo12:\symbol
  .else
    adrp \reg, \symbol
    add \reg, \reg, #:lo12:\symbol
  .endif
.endm  // adr_g


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
.endm // movlit
