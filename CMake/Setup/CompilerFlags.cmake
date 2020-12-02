message(STATUS "detected c compiler id: ${CMAKE_C_COMPILER_ID}, version: ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "detected c++ compiler id: ${CMAKE_CXX_COMPILER_ID}, version: ${CMAKE_CXX_COMPILER_VERSION}")


macro(read_board_compiler_flags_from_file IN_FILE OUT_VAR)
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

if (CMAKE_C_COMPILER_ID AND NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message(WARNING "clang will be the only supported compiler in the near future")
endif ()

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(WARNING "clang will be the only supported compiler in the near future")
endif ()


macro(setup_compiler_flags)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_LINK_FLAGS "")

    read_board_compiler_flags_from_file(${MOSS_SOURCE_CODE_DIR}/Board/${BOARD}/.flags BOARD_COMPILER_FLAGS)
    message(STATUS "BOARD_COMPILER_FLAGS is: ${BOARD_COMPILER_FLAGS}")
    # set BOARD_COMPILER_FLAGS here
    string(JOIN " " CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${BOARD_COMPILER_FLAGS})

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
