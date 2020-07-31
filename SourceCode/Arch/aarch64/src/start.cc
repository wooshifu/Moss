//
// Created by shifu on 2021/10/14.
//

#include "aarch64/start.hh"

asm(R"asm(
.section .text.boot
_text_boot:
  b _start
)asm");

extern "C" [[gnu::used]] void _start() {
  asm(R"asm(
    b .
  )asm"
      :
      :
      :);
}
