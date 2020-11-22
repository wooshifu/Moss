cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

message(STATUS "cmake version: ${CMAKE_VERSION}")

# skip cmake compiler check
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

project(Moss LANGUAGES CXX ASM)

if (${Moss_SOURCE_DIR} STREQUAL ${Moss_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif ()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile commands can be used to help clang-tidy code analysis

set(MOSS_SOURCE_CODE_DIR ${Moss_SOURCE_DIR}/SourceCode)

include(${Moss_SOURCE_DIR}/CMake/ArchDetector.cmake)
include(${Moss_SOURCE_DIR}/CMake/VarsDump.cmake)
include(${Moss_SOURCE_DIR}/CMake/HeaderFiles.cmake)
include(${Moss_SOURCE_DIR}/CMake/Utils.cmake)


if (MossArch STREQUAL arm)
    set(CMAKE_C_FLAGS "-mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7")
elseif (MossArch STREQUAL arm64)
    set(CMAKE_C_FLAGS "-march=armv8-a+crc -mcpu=cortex-a53")
else ()
    message(FATAL_ERROR "unexpected ARCH ${MossArch}")
endif ()

set(IGNORE_SPECIFIC_WARNINGS "-Wno-unused-variable -Wno-unused-parameter -Wno-unused-function")
# todo: optimization level at Release mode
set(COMMON_CMAKE_C_FLAGS "-save-temps -O0 -g -Wall -Wextra -Werror ${IGNORE_SPECIFIC_WARNINGS} -MD -fpic -ffreestanding -nostdlib -nostartfiles")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_CMAKE_C_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}")
# define __CURRENT_FILE_NAME__, log function will use this macro to output the file name
# todo: refactor __CURRENT_FILE_NAME__, using constexpr impl
set(CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_C_FLAGS} -D__CURRENT_FILE_NAME__='\"$(notdir $(abspath $<))\"'")
# todo: considering -std=gnu17
# about c17, see https://stackoverflow.com/questions/47529854/what-is-c17-and-what-changes-have-been-made-to-the-language
set(CMAKE_C_FLAGS "-std=gnu11 ${CMAKE_C_FLAGS} -D__CURRENT_FILE_NAME__='\"$(notdir $(abspath $<))\"'")

message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_ASM_FLAGS: ${CMAKE_ASM_FLAGS}")
message(STATUS "TOOLCHAIN_PREFIX: ${TOOLCHAIN_PREFIX}")
message(STATUS "CMAKE_OBJCOPY: ${CMAKE_OBJCOPY}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


set(KernelName Kernel.elf)

# make config.h to be available
include_directories(${MOSS_SOURCE_CODE_DIR}/Config/include)
add_subdirectory(${MOSS_SOURCE_CODE_DIR})

dump_cmake_variables()

