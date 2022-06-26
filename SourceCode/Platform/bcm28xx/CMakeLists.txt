project(ObjPlatBCM28xx CXX)

aux_source_directory(${ObjPlatBCM28xx_SOURCE_DIR}/src ObjPlatBCM28xxSrc)
add_library(${PROJECT_NAME}
        OBJECT
        ${ObjPlatBCM28xxSrc})

target_include_libcxx_header_files(${PROJECT_NAME})
target_include_libstd_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})
target_include_bcm28xx_header_files(${PROJECT_NAME})
target_include_kernel_header_files(${PROJECT_NAME})
target_enable_iwyu(${PROJECT_NAME})
