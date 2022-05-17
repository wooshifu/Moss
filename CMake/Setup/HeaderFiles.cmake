function(include_kconfig_header_file_globally flags)
    set(${flags} "-imacros ${Moss_SOURCE_DIR}/SourceCode/Config/include/kconfig.hh" PARENT_SCOPE)
endfunction()

function(target_include_hal_header_files IN_target)
    target_include_directories(${IN_target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/HAL/include)
endfunction()

function(target_include_libcxx_header_files IN_target)
    target_include_directories(${IN_target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Libraries/lib_cxx/include)
endfunction()

function(target_include_libstdcxx_header_files IN_target)
    target_include_directories(${IN_target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Libraries/lib_compatible/include)
    target_include_directories(${IN_target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Libraries/lib_std/include)
endfunction()

function(target_include_kernel_header_files IN_target)
    target_include_directories(${IN_target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Kernel/include)
endfunction()

function(target_include_specific_arch_header_files IN_target IN_arch)
    target_include_directories(${IN_target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Arch/${IN_arch}/include)
endfunction()

function(target_include_rpi3_header_files IN_target)
    target_include_directories(${IN_target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Board/rpi3/include)
endfunction()

function(target_include_tests_header_files IN_target)
    target_include_directories(${IN_target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Tests/include)
endfunction()

function(target_include_arch_header_files IN_target)
    target_include_directories(${IN_target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Arch/include)
endfunction()