//
// Created by shifu on 2021/10/14.
//

#include "aarch64/start.hh"

asm(R"asm(
.section .text.boot
_text_boot:
  b start
)asm");

extern "C" [[gnu::used]] void start() {
  asm(R"asm(
    b .
  )asm"
      :
      :
      :);
}
