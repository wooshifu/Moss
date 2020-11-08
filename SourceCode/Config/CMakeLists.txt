project(Config LANGUAGES CXX DESCRIPTION "Configurations of this project")


#######################################################################
option(TARGET_QEMU "run os on qemu" ON)
message("option TARGET_QEMU is: ${TARGET_QEMU}")

option(TARGET_RASPBERRY_PI3 "run os on raspberry pi3")
message("option TARGET_RASPBERRY_PI3 is: ${TARGET_RASPBERRY_PI3}")
#######################################################################


configure_file(${Config_SOURCE_DIR}/include/config.h.in
        ${Config_SOURCE_DIR}/include/config.h
        @ONLY)
