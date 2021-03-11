message(STATUS "detected c compiler id: ${CMAKE_C_COMPILER_ID}, version: ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "detected c++ compiler id: ${CMAKE_CXX_COMPILER_ID}, version: ${CMAKE_CXX_COMPILER_VERSION}")

if (CMAKE_C_COMPILER_ID AND NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "[compiler] only gcc is supported")
endif ()

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "[compiler] only g++ is supported")
endif ()

set(GCC_MIN_VERSION_REQUIRED 10.0.0)
if (CMAKE_C_COMPILER_ID AND CMAKE_C_COMPILER_VERSION VERSION_LESS ${GCC_MIN_VERSION_REQUIRED} OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${GCC_MIN_VERSION_REQUIRED})
    message(FATAL_ERROR "gcc version must be greater than ${GCC_MIN_VERSION_REQUIRED}")
endif ()

function(read_arch_compiler_flags_from_file IN_file OUT_flag)
    file(STRINGS ${IN_file} lines)

    foreach (line IN LISTS lines)
        # "#" started line is comment line, remove it
        string(REGEX REPLACE "\#.*" "" stripped "${line}")
        if (stripped)
            list(APPEND flag "${stripped}")
        endif ()
    endforeach ()
    message(STATUS ".flag: ${flag}")
    set(${OUT_flag} ${flag} PARENT_SCOPE)
endfunction()

macro(disable_compiler_link_flags)
    set(CMAKE_C_LINK_FLAGS "")
    set(CMAKE_CXX_LINK_FLAGS "")
endmacro()

function(get_optimization_level OUT_optimization_level)
    if (NOT DEFINED OPTIMIZATION_LEVEL)
        if (CMAKE_BUILD_TYPE MATCHES "Rel")
            set(${OUT_optimization_level} 2 PARENT_SCOPE)
        elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
            set(${OUT_optimization_level} 0 PARENT_SCOPE)
        else ()
            set(${OUT_optimization_level} 0 PARENT_SCOPE)
        endif ()
    endif ()
endfunction()

function(get_debug_flag OUT_debug_flag)
    if (CMAKE_BUILD_TYPE MATCHES "Rel")
        set(${OUT_debug_flag} "" PARENT_SCOPE)
    else ()
        set(${OUT_debug_flag} "-g" PARENT_SCOPE)
    endif ()
endfunction()

function(setup_compiler_flags IN_board IN_arch)
    set(arch_flag_file ${MOSS_SOURCE_CODE_DIR}/Arch/${ARCH}/.flags)
    if (NOT EXISTS ${arch_flag_file})
        message(FATAL_ERROR "${arch_flag_file} must exist")
    endif ()

    message(STATUS "[arch] .flag file: ${arch_flag_file}")
    read_arch_compiler_flags_from_file(${arch_flag_file} arch_compiler_flags)
    message(STATUS "[arch] arch_compiler_flags is: ${arch_compiler_flags}")

    set(board_flag_file ${MOSS_SOURCE_CODE_DIR}/Board/${IN_board}/.flags)
    if (EXISTS ${board_flag_file})
        message(STATUS "[board] .flag file: ${board_flag_file}")
        read_arch_compiler_flags_from_file(${board_flag_file} board_compiler_flags)
        message(STATUS "[board] board_compiler_flags is: ${board_compiler_flags}")
    endif ()


    set(ignore_specific_warnings
            "-Wno-unused-variable"
            "-Wno-unused-parameter"
            "-Wno-unused-function"
            )

    set(c_ignore_specific_warnings "-Wno-override-init")

    get_debug_flag(debug_flag)
    message(STATUS "debug_flag :${debug_flag}")
    get_optimization_level(optimization_level)
    message(STATUS "OPTIMIZATION_LEVEL :${optimization_level}")

    set(macro_flags
            "-D_GLIBCXX_HOSTED=0" # override glibc macro
            )

    set(cxx_specific_flags "-nostdinc++")

    string(JOIN " " common_compiler_flags
            ${arch_compiler_flags}
            ${board_compiler_flags}
            # "-v"
            "-O${optimization_level}"
            ${debug_flag}
            ${macro_flags}
            # "-save-temps" # this flag will broke iwyu
            "-Wall"
            "-Wextra"
            "-Werror"
            ${ignore_specific_warnings}
            "-MD"
            "-fpic"
            "-ffreestanding"
            "-fno-builtin"
            "-fno-exceptions"
            "-nostdinc"
            "-nostdlib"
            "-nostartfiles"
            )

    set(CMAKE_C_FLAGS "-std=c11 ${c_ignore_specific_warnings} ${common_compiler_flags}" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS "-std=c++20 ${cxx_specific_flags} ${common_compiler_flags}" PARENT_SCOPE)
    set(CMAKE_ASM_FLAGS "-std=c11 ${common_compiler_flags}" PARENT_SCOPE)
endfunction()

macro(print_compiler_flags)
    string(REPEAT "=" 300 compiler_flags_separator)
    message(STATUS "${compiler_flags_separator}")
    message(STATUS "${compiler_flags_separator}")
    message(STATUS "CMAKE_ASM_FLAGS:  ${CMAKE_ASM_FLAGS}")
    message(STATUS "CMAKE_C_FLAGS:    ${CMAKE_C_FLAGS}")
    message(STATUS "CMAKE_CXX_FLAGS:  ${CMAKE_CXX_FLAGS}")
    message(STATUS "CMAKE_OBJCOPY:    ${CMAKE_OBJCOPY}")
    message(STATUS "${compiler_flags_separator}")
    message(STATUS "${compiler_flags_separator}")
endmacro()
