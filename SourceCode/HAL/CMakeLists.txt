project(ObjHAL LANGUAGES CXX DESCRIPTION "HAL, also known as hardware abstract layer")

aux_source_directory(${ObjHAL_SOURCE_DIR}/src ObjHALSrc)
add_library(${PROJECT_NAME}
        OBJECT
        ${ObjHALSrc})

target_include_raspberry_pi3_header_files(${PROJECT_NAME})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_libstdcxx_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_specific_arch_header_files(${PROJECT_NAME} aarch64)

target_enable_iwyu(${PROJECT_NAME})
