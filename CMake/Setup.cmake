message(STATUS "CMAKE_C_COMPILER_ID:${CMAKE_C_COMPILER_ID} CMAKE_CXX_COMPILER_ID:${CMAKE_CXX_COMPILER_ID}")
# NOTE: define global variable USING_GNU_COMPILER or USING_CLANG_COMPILER for following usage
if (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
    message(STATUS "using GCC compiler")
    set(USING_GNU_COMPILER 1)
elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message(STATUS "using clang compiler")
    set(USING_CLANG_COMPILER 1)
else ()
    message(FATAL_ERROR "unsupported compiler: CMAKE_C_COMPILER_ID:${CMAKE_C_COMPILER_ID} CMAKE_CXX_COMPILER_ID:${CMAKE_CXX_COMPILER_ID}")
endif ()

include(${Moss_SOURCE_DIR}/CMake/Options.cmake)
# find all cmake files, and then include the file
file(GLOB cmake_files LIST_DIRECTORIES false ${Moss_SOURCE_DIR}/CMake/Setup/*.cmake)
message(STATUS "found CmakeFiles: ${cmake_files}")
foreach (cmake_file ${cmake_files})
    message(STATUS "include ${cmake_file}")
    include(${cmake_file})
endforeach ()

message(STATUS "********** setup began **********")

message(STATUS ">>> setting up kconfig")
setup_kconfig()

setup_board(CONFIG_BOARD)
message(STATUS ">>> building board: ${CONFIG_BOARD}")

setup_arch(${CONFIG_BOARD} ARCH)
message(STATUS ">>> building arch: ${ARCH}")

disable_compiler_link_flags()
setup_compiler_flags(${CONFIG_BOARD} ${ARCH})
print_compiler_flags()

message(STATUS "********** setup ended **********")

dump_cmake_variables()
