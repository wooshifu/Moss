message(STATUS "detected c compiler id: ${CMAKE_C_COMPILER_ID}, version: ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "detected c++ compiler id: ${CMAKE_CXX_COMPILER_ID}, version: ${CMAKE_CXX_COMPILER_VERSION}")

if (CMAKE_C_COMPILER_ID AND NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message(WARNING "clang will be the only supported compiler in the near future")
endif ()

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(WARNING "clang will be the only supported compiler in the near future")
endif ()


macro(setup_compiler_flags)
    set(CMAKE_CXX_LINK_FLAGS "")

    if (MossArch STREQUAL arm)
        string(JOIN " " CMAKE_C_FLAGS
                "-mfpu=neon-vfpv4"
                "-mfloat-abi=hard"
                "-march=armv7-a"
                "-mtune=cortex-a7"
                )
    elseif (MossArch STREQUAL arm64)
        string(JOIN " " CMAKE_C_FLAGS
                ${CMAKE_C_FLAGS}
                "--target=aarch64-unknown-linux-elf"
                "-march=armv8-a+crc"
                "-mcpu=cortex-a53"
                )
    else ()
        message(FATAL_ERROR "unexpected ARCH ${MossArch}")
    endif ()

    string(JOIN " " IGNORE_SPECIFIC_WARNINGS
            "-Wno-unused-variable"
            "-Wno-unused-parameter"
            "-Wno-unused-function"
            "-Wno-unused-command-line-argument"
            )

    # todo: optimization level at Release mode
    string(JOIN " " COMMON_CMAKE_C_FLAGS
            "-v"
            "-O0" #todo: fix clang must be O1
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

    # define __CURRENT_FILE_NAME__, log function will use this macro to output the file name
    # todo: refactor __CURRENT_FILE_NAME__, using constexpr impl
    string(JOIN " " CMAKE_CXX_FLAGS
            ${CMAKE_CXX_FLAGS}
            "-std=c++20"
            "${CMAKE_C_FLAGS}"
            "-D__CURRENT_FILE_NAME__='\"$(notdir $(abspath $<))\"'"
            )

    string(JOIN " " CMAKE_C_FLAGS
            "-std=c11"
            "${CMAKE_C_FLAGS}"
            "-D__CURRENT_FILE_NAME__='\"$(notdir $(abspath $<))\"'"
            )

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
