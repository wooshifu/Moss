.global halt
halt:
    mov w0, 0x18
    mov x1, #0x20000
    add x1, x1, #0x26
    hlt #0xF000
