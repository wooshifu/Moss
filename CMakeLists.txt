cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

message(STATUS "cmake version: ${CMAKE_VERSION}")

# skip cmake compiler check
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

project(Moss LANGUAGES C CXX ASM)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "cmake command line variable CMAKE_TOOLCHAIN_FILE should be defined to specify the compiler toolchain\n"
            "cmake command line should be: cmake -DCMAKE_TOOLCHAIN_FILE=${Moss_SOURCE_DIR}/CMake/Toolchain/ToolchainClang.cmake")
    message(FATAL_ERROR "")
endif ()

if (${Moss_SOURCE_DIR} STREQUAL ${Moss_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile commands can be used to help clang-tidy code analysis

set(MOSS_SOURCE_CODE_DIR ${Moss_SOURCE_DIR}/SourceCode)
set(MOSS_SCRIPTS_DIR ${Moss_SOURCE_DIR}/Scripts)

set(KernelName kernel.elf)

include(${Moss_SOURCE_DIR}/CMake/Setup.cmake)

# make config.h to be available globally
include_directories(${MOSS_SOURCE_CODE_DIR}/Config/include)
include_directories(${MOSS_SOURCE_CODE_DIR}/Libraries/libtest/include)
add_subdirectory(${MOSS_SOURCE_CODE_DIR})
