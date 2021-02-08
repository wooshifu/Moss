message(STATUS "using aarch64-unknown-elf, version 10.2.0")

set(CMAKE_SYSTEM_NAME Darwin)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(arm_gcc_toolchain_file_name aarch64-unknown-elf.tar.gz)
set(arm_gcc_toolchain_path $ENV{HOME}/${arm_gcc_toolchain_file_name})
set(arm_gcc_toolchain_dir $ENV{HOME}/aarch64-unknown-elf)
set(arm_gcc_toolchain_url https://github.com/wooshifu/gcc-aarch64-mac/releases/download/0.0.1/aarch64-unknown-elf.tar.gz)

if (NOT EXISTS ${arm_gcc_toolchain_dir})
    if (NOT EXISTS ${arm_gcc_toolchain_path})
        message(STATUS "downloading ${arm_gcc_toolchain_url}, wait a moment")
        file(DOWNLOAD ${arm_gcc_toolchain_url} ${arm_gcc_toolchain_path}
                EXPECTED_MD5 a08ce261e45ef692acfe525cc43872f1
                TLS_VERIFY ON
                SHOW_PROGRESS)
    endif ()

    file(MAKE_DIRECTORY ${arm_gcc_toolchain_dir})
    message(STATUS "extracting ${arm_gcc_toolchain_path}, wait a moment")
    execute_process(COMMAND
            ${CMAKE_COMMAND} -E tar xvzf ${arm_gcc_toolchain_path}
            WORKING_DIRECTORY $ENV{HOME})
endif ()


set(CMAKE_FIND_ROOT_PATH ${arm_gcc_toolchain_dir}/bin)

set(CMAKE_C_COMPILER ${CMAKE_FIND_ROOT_PATH}/aarch64-unknown-elf-gcc)
set(CMAKE_CXX_COMPILER ${CMAKE_FIND_ROOT_PATH}/aarch64-unknown-elf-g++)

set(CMAKE_CROSSCOMPILING_EMULATOR qemu-system-aarch64)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(TOOLCHAIN_PREFIX aarch64-unknown-linux-gnu-)
set(CMAKE_OBJCOPY ${CMAKE_FIND_ROOT_PATH}/aarch64-unknown-elf-objcopy)
