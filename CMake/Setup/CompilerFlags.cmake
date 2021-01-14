message(STATUS "detected c compiler id: ${CMAKE_C_COMPILER_ID}, version: ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "detected c++ compiler id: ${CMAKE_CXX_COMPILER_ID}, version: ${CMAKE_CXX_COMPILER_VERSION}")

if (CMAKE_C_COMPILER_ID AND NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "clang is the only supported compiler")
endif ()

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "clang is the only supported compiler")
endif ()

set(CLANG_MIN_VERSION_REQUIRED 10.0.0)
if (CMAKE_C_COMPILER_ID AND CMAKE_C_COMPILER_VERSION VERSION_LESS ${CLANG_MIN_VERSION_REQUIRED} OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${CLANG_MIN_VERSION_REQUIRED})
    message(FATAL_ERROR "Clang version must be greater than ${CLANG_MIN_VERSION_REQUIRED}")
endif ()

macro(read_arch_compiler_flags_from_file IN_FILE OUT_VAR)
    file(STRINGS ${IN_FILE} LINES)

    foreach (LINE IN LISTS LINES)
        # "#" started line is comment line, remove it
        string(REGEX REPLACE "\#.*" "" STRIPPED "${LINE}")
        if (STRIPPED)
            list(APPEND ${OUT_VAR} "${STRIPPED}")
        endif ()
    endforeach ()
endmacro()


if (CMAKE_BUILD_TYPE MATCHES "Rel")
    set(OPTIMIZATION_LEVEL 2)
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(OPTIMIZATION_LEVEL 0)
else ()
    set(OPTIMIZATION_LEVEL 0)
endif ()
message(STATUS "OPTIMIZATION_LEVEL: ${OPTIMIZATION_LEVEL}")


# todo: use c++20 module feature, but cmake currently have no support for this feature
#"-emit-module-interface"
#"-emit-module"
#"-fmodules"
#"-fsystem-module"
#"-fbuiltin-module-map"
#"-fimplicit-module-maps"

macro(setup_compiler_flags)
    set(CMAKE_CXX_LINK_FLAGS "")

    read_arch_compiler_flags_from_file(${MOSS_SOURCE_CODE_DIR}/Arch/${ARCH}/.flags ARCH_COMPILER_FLAGS)
    message(STATUS "ARCH_COMPILER_FLAGS is: ${ARCH_COMPILER_FLAGS}")
    # set ARCH_COMPILER_FLAGS here
    string(JOIN " " CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${ARCH_COMPILER_FLAGS})

    string(JOIN " " IGNORE_SPECIFIC_WARNINGS
            "-Wno-unused-variable"
            "-Wno-unused-parameter"
            "-Wno-unused-function"
            "-Wno-unused-command-line-argument"
            )

    string(JOIN " " COMMON_CMAKE_C_FLAGS
            # "-v"
            "-O${OPTIMIZATION_LEVEL}"
            "-g"
            "-save-temps"
            "-Wall"
            "-Wextra"
            "-Werror"
            "${IGNORE_SPECIFIC_WARNINGS}"
            "-MD"
            "-fuse-ld=lld"
            "-fpic"
            "-ffreestanding"
            "-fno-builtin"
            "-fno-exceptions"
            "-nostdinc"
            "-nostdinc++"
            "-nostdlib"
            "-nostartfiles"
            "-Wl,--no-dynamic-linker"
            "-Wl,--nostdlib"
            "-Wl,-error-limit=0"
            "-Wl,-v"
            )

    string(JOIN " " CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" "${COMMON_CMAKE_C_FLAGS}")
    string(JOIN " " CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++20" "${CMAKE_C_FLAGS}")
    string(JOIN " " CMAKE_C_FLAGS "-std=c11" "${CMAKE_C_FLAGS}")
    string(JOIN " " CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS} "${CMAKE_C_FLAGS}")

    string(REPEAT "=" 300 COMPILER_FLAGS_SEPARATOR)
    message(STATUS "${COMPILER_FLAGS_SEPARATOR}")
    message(STATUS "${COMPILER_FLAGS_SEPARATOR}")
    message(STATUS "CMAKE_C_FLAGS:    ${CMAKE_C_FLAGS}")
    message(STATUS "CMAKE_CXX_FLAGS:  ${CMAKE_CXX_FLAGS}")
    message(STATUS "CMAKE_ASM_FLAGS:  ${CMAKE_ASM_FLAGS}")
    message(STATUS "CMAKE_OBJCOPY:    ${CMAKE_OBJCOPY}")
    message(STATUS "${COMPILER_FLAGS_SEPARATOR}")
    message(STATUS "${COMPILER_FLAGS_SEPARATOR}")
endmacro()
