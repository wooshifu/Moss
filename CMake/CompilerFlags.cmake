macro(setup_compiler_flags)
    if (MossArch STREQUAL arm)
        string(JOIN " " CMAKE_C_FLAGS
                "-mfpu=neon-vfpv4"
                "-mfloat-abi=hard"
                "-march=armv7-a"
                "-mtune=cortex-a7")
    elseif (MossArch STREQUAL arm64)
        string(JOIN " " CMAKE_C_FLAGS
                ${CMAKE_C_FLAGS}
                "-march=armv8-a+crc"
                "-mcpu=cortex-a53")
    else ()
        message(FATAL_ERROR "unexpected ARCH ${MossArch}")
    endif ()

    string(JOIN " " IGNORE_SPECIFIC_WARNINGS
            "-Wno-unused-variable"
            "-Wno-unused-parameter"
            "-Wno-unused-function")

    # todo: optimization level at Release mode
    string(JOIN " " COMMON_CMAKE_C_FLAGS
            "-save-temps"
            "-O0"
            "-g"
            "-Wall"
            "-Wextra"
            "-Werror"
            "${IGNORE_SPECIFIC_WARNINGS}"
            "-MD"
            "-fpic"
            "-ffreestanding"
            "-nostdlib"
            "-nostartfiles")

    string(JOIN " " CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" "${COMMON_CMAKE_C_FLAGS}")

    # define __CURRENT_FILE_NAME__, log function will use this macro to output the file name
    # todo: refactor __CURRENT_FILE_NAME__, using constexpr impl
    string(JOIN " " CMAKE_CXX_FLAGS
            ${CMAKE_CXX_FLAGS}
            "-std=c++2a"
            "${CMAKE_C_FLAGS}"
            "-D__CURRENT_FILE_NAME__='\"$(notdir $(abspath $<))\"'")

    # todo: considering -std=gnu17
    # about c17, see https://stackoverflow.com/questions/47529854/what-is-c17-and-what-changes-have-been-made-to-the-language
    string(JOIN " " CMAKE_C_FLAGS
            "-std=gnu11"
            "${CMAKE_C_FLAGS}"
            "-D__CURRENT_FILE_NAME__='\"$(notdir $(abspath $<))\"'")

    string(JOIN " " CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS} "${CMAKE_C_FLAGS}")

    string(REPEAT "=" 300 COMPILER_FLAGS_SEPARATOR)
    message(STATUS "${COMPILER_FLAGS_SEPARATOR}")
    message(STATUS "${COMPILER_FLAGS_SEPARATOR}")
    message(STATUS "CMAKE_C_FLAGS:    ${CMAKE_C_FLAGS}")
    message(STATUS "CMAKE_CXX_FLAGS:  ${CMAKE_CXX_FLAGS}")
    message(STATUS "CMAKE_ASM_FLAGS:  ${CMAKE_ASM_FLAGS}")
    message(STATUS "TOOLCHAIN_PREFIX: ${TOOLCHAIN_PREFIX}")
    message(STATUS "CMAKE_OBJCOPY:    ${CMAKE_OBJCOPY}")
    message(STATUS "${COMPILER_FLAGS_SEPARATOR}")
    message(STATUS "${COMPILER_FLAGS_SEPARATOR}")
endmacro()
