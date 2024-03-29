set(CLANG_MIN_VERSION_REQUIRED 14.0.0)
if (USING_CLANG_COMPILER)
    if (CMAKE_C_COMPILER_ID AND CMAKE_C_COMPILER_VERSION VERSION_LESS ${CLANG_MIN_VERSION_REQUIRED} OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${CLANG_MIN_VERSION_REQUIRED})
        message(FATAL_ERROR "clang version must be greater than ${CLANG_MIN_VERSION_REQUIRED}")
    endif ()
else ()
    message(FATAL_ERROR "unsupported compiler")
endif ()

function(read_compiler_flags_from_flags_cmake_file IN_file OUT_flag)
    if (NOT EXISTS ${IN_file})
        message(FATAL_ERROR "file ${IN_file} not found")
    endif ()

    include(${IN_file})
    message(STATUS "NOTE: by convention, function(get_compiler_flags OUT_flags) should be defined in ${IN_file}")
    get_compiler_flags(compiler_flags)
    message(STATUS "compiler flags: ${compiler_flags}")

    set(${OUT_flag} ${compiler_flags} PARENT_SCOPE)
endfunction()

macro(disable_compiler_link_flags)
    set(CMAKE_C_LINK_FLAGS "")
    set(CMAKE_CXX_LINK_FLAGS "")
endmacro()

function(get_optimization_level OUT_optimization_level)
    if (NOT DEFINED OPTIMIZATION_LEVEL)
        if (CMAKE_BUILD_TYPE MATCHES "Rel")
            set(${OUT_optimization_level} 3 PARENT_SCOPE)
        elseif (CMAKE_BUILD_TYPE MATCHES "Deb") # RelWithDebInfo mode
            set(${OUT_optimization_level} 0 PARENT_SCOPE)
        else ()
            set(${OUT_optimization_level} 0 PARENT_SCOPE)
        endif ()
    endif ()
endfunction()

function(get_debug_flag OUT_debug_flag)
    if (CMAKE_BUILD_TYPE MATCHES "Deb") # RelWithDebInfo mode
        set(${OUT_debug_flag} "-g3" PARENT_SCOPE)
    else ()
        set(${OUT_debug_flag} "-g0" PARENT_SCOPE)
    endif ()
endfunction()

function(setup_compiler_flags IN_project IN_arch)
    set(arch_flag_file ${MOSS_SOURCE_CODE_DIR}/Arch/${ARCH}/flags.cmake)
    message(STATUS "[arch] flags.cmake file: ${arch_flag_file}")
    read_compiler_flags_from_flags_cmake_file(${arch_flag_file} arch_compiler_flags)
    message(STATUS "[arch] arch_compiler_flags is: ${arch_compiler_flags}")

    set(project_flag_file ${MOSS_SOURCE_CODE_DIR}/Project/${IN_project}/flags.cmake)
    message(STATUS "[project] flags.cmake file: ${project_flag_file}")
    read_compiler_flags_from_flags_cmake_file(${project_flag_file} project_compiler_flags)
    message(STATUS "[project] project_compiler_flags is: ${project_compiler_flags}")

    set(ignore_specific_warnings "-Wno-gnu-zero-variadic-macro-arguments")

    get_debug_flag(debug_flag)
    message(STATUS "debug_flag: ${debug_flag}")
    get_optimization_level(optimization_level)
    message(STATUS "optimization_level: ${optimization_level}")

    set(macro_flags "")

    include_kconfig_header_file_globally(kconfig_flags)
    message(STATUS kconfig_flags: "${kconfig_flags}")

    string(JOIN " " common_compiler_flags
            ${kconfig_flags}
            ${arch_compiler_flags}
            ${project_compiler_flags}
            # "-v"
            "-O${optimization_level}"
            ${debug_flag}
            ${macro_flags}
            "-Wall"
            "-Wextra"
            "-Wpedantic"
            "-Werror"
            ${ignore_specific_warnings}
            "-MD"
            "-fPIC"
            "-fPIE"
            "-ffreestanding"
            "-fno-builtin"
            "-fno-exceptions"
            "-fno-common"
            # "-save-temps" # this flag will cause inline assembly code inclusion failed, run `Script/fix_asm_includes.py` to change the include path to absolute path
            )

    set(CMAKE_C_FLAGS "-std=c11 -nostdinc -nostdlibinc -nostdinc++ -nostdlib++ ${common_compiler_flags}" CACHE INTERNAL "" FORCE)
    set(CMAKE_CXX_FLAGS "-std=c++20 -nostdlib++ ${common_compiler_flags}" CACHE INTERNAL "" FORCE)
    set(CMAKE_ASM_FLAGS "-std=c11 -DASSEMBLY ${common_compiler_flags}" CACHE INTERNAL "" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS "${common_compiler_flags} -fuse-ld=lld -nostdlib" CACHE INTERNAL "" FORCE)
endfunction()

macro(print_compiler_flags)
    string(REPEAT "=" 300 compiler_flags_separator)
    message(STATUS "${compiler_flags_separator}")
    message(STATUS "${compiler_flags_separator}")
    message(STATUS "CMAKE_ASM_FLAGS:  ${CMAKE_ASM_FLAGS}")
    message(STATUS "CMAKE_C_FLAGS:    ${CMAKE_C_FLAGS}")
    message(STATUS "CMAKE_CXX_FLAGS:  ${CMAKE_CXX_FLAGS}")
    message(STATUS "CMAKE_EXE_LINKER_FLAGS:  ${CMAKE_EXE_LINKER_FLAGS}")
    message(STATUS "CMAKE_OBJCOPY:    ${CMAKE_OBJCOPY}")
    message(STATUS "${compiler_flags_separator}")
    message(STATUS "${compiler_flags_separator}")
endmacro()
