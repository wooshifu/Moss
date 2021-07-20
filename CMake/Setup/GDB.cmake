if (NOT DISABLE_GDBINIT)
    file(WRITE $ENV{HOME}/.gdbinit "add-symbol-file ${CMAKE_BINARY_DIR}/bin/kernel.elf 0xffffffff00000000")
    message(STATUS ".gdbinit has been copied to $ENV{HOME}, if you don't want to copy this .gdbinit file define add -DDISABLE_GDBINIT=1 to cmake parameter")
endif ()
