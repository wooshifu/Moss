message(STATUS "CMAKE_C_COMPILER_ID:${CMAKE_C_COMPILER_ID} CMAKE_CXX_COMPILER_ID:${CMAKE_CXX_COMPILER_ID}")
if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
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

setup_project(CONFIG_PROJECT)
message(STATUS ">>> building project: ${CONFIG_PROJECT}")

setup_arch(${CONFIG_PROJECT} ARCH)
message(STATUS ">>> building arch: ${ARCH}")

disable_compiler_link_flags()
setup_compiler_flags(${CONFIG_PROJECT} ${ARCH})
print_compiler_flags()

message(STATUS "********** setup ended **********")

dump_cmake_variables()
