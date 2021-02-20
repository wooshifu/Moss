#include "libcxx/log.hh"

.global halt
halt:
    mov w0, 0x18
    mov x1, #0x20000
    add x1, x1, #0x26
    hlt #0xF000


.global never_return
never_return:
#if LOG_LEVEL <= LOG_LEVEL_DEBUG
    ldr x0, =never_return_message
    bl printf
#endif
    wfi
    b never_return

never_return_message:
    .asciz "[DEBUG halt.S:21(never_return)] wfi, will never return\n"
