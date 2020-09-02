project(ObjKernel C ASM)

aux_source_directory(${ObjKernel_SOURCE_DIR}/src ObjKernelSrc)

add_library(${PROJECT_NAME}
        OBJECT
        ${ObjKernelSrc})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_libc_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})
# todo: remove header dependency
target_include_arch_header_files(${PROJECT_NAME})
target_include_raspberry_pi3_header_files(${PROJECT_NAME})
