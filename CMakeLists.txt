cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

message(STATUS "cmake version: ${CMAKE_VERSION}")

# skip cmake compiler check
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

project(Moss LANGUAGES CXX ASM)

if (${Moss_SOURCE_DIR} STREQUAL ${Moss_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile commands can be used to help clang-tidy code analysis

set(MOSS_SOURCE_CODE_DIR ${Moss_SOURCE_DIR}/SourceCode)

set(KernelName Kernel.elf)

include(${Moss_SOURCE_DIR}/CMake/Setup.cmake)

# make config.h to be available
include_directories(${MOSS_SOURCE_CODE_DIR}/Config/include)
add_subdirectory(${MOSS_SOURCE_CODE_DIR})
