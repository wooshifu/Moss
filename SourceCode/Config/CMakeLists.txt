project(Config LANGUAGES CXX DESCRIPTION "Configurations of this project")


#######################################################################
option(CONFIG_TARGET_QEMU "run os on qemu" ON)
message(STATUS "option TARGET_QEMU is: ${CONFIG_TARGET_QEMU}")

option(CONFIG_TARGET_QEMU "run os on raspberry pi3")
message(STATUS "option TARGET_RASPBERRY_PI3 is: ${CONFIG_TARGET_QEMU}")
#######################################################################


configure_file(${Config_SOURCE_DIR}/include/config.hh.in
        ${Config_SOURCE_DIR}/include/config.hh
        @ONLY)
