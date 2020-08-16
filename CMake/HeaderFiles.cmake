function(target_include_hal_header_files Target)
    target_include_directories(${Target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/HAL/include)
endfunction()

function(target_include_libc_header_files Target)
    target_include_directories(${Target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Libraries/LibC/include)
endfunction()

function(target_include_kernel_header_files Target)
    target_include_directories(${Target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Kernel/include)
endfunction()

function(target_include_tests_header_files Target)
    target_include_directories(${Target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Tests/include)
endfunction()
