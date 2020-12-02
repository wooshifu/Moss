function(target_include_hal_header_files Target)
    target_include_directories(${Target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/HAL/include)
endfunction()

function(target_include_libcxx_header_files Target)
    target_include_directories(${Target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Libraries/LibCXX/include)
endfunction()

function(target_include_kernel_header_files Target)
    target_include_directories(${Target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Kernel/include)
endfunction()

function(target_include_arch_header_files Target)
    target_include_directories(${Target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Arch/${ARCH}/include)
endfunction()

function(target_include_raspberry_pi3_header_files Target)
    target_include_directories(${Target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Board/RaspberryPi3/include)
endfunction()

function(target_include_tests_header_files Target)
    target_include_directories(${Target} PRIVATE ${MOSS_SOURCE_CODE_DIR}/Tests/include)
endfunction()
