project(ObjKernel CXX ASM)

aux_source_directory(${ObjKernel_SOURCE_DIR}/src ObjKernelSrc)

add_library(${PROJECT_NAME}
        OBJECT
        ${ObjKernelSrc})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_libio_header_files(${PROJECT_NAME})
target_include_libstd_header_files(${PROJECT_NAME})
target_include_libhal_header_files(${PROJECT_NAME})

target_enable_iwyu(${PROJECT_NAME})
