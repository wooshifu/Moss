set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

message(STATUS "CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")

if (CMAKE_HOST_SYSTEM_NAME STREQUAL Darwin)
    set(CMAKE_FIND_ROOT_PATH /usr/local/opt/llvm/bin)
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL Linux)
    set(CMAKE_FIND_ROOT_PATH /usr/bin)
else ()
    set(CMAKE_FIND_ROOT_PATH /usr/bin)
endif ()

set(CMAKE_OBJCOPY llvm-objcopy)

# remove macos --sysroot compile flags
set(CMAKE_OSX_SYSROOT "")
set(CMAKE_CXX_SYSROOT_FLAG "" FORCE)
