project(ObjKernel CXX ASM)

aux_source_directory(${ObjKernel_SOURCE_DIR}/src ObjKernelSrc)

add_library(${PROJECT_NAME}
        OBJECT
        ${ObjKernelSrc})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_libstdcxx_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})

target_enable_iwyu(${PROJECT_NAME})
