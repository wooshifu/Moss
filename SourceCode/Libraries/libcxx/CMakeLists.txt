project(ObjLibCXX CXX ASM)

aux_source_directory(${ObjLibCXX_SOURCE_DIR}/src LibCXXSrc)

add_library(${PROJECT_NAME}
        OBJECT
        ${LibCXXSrc})
target_include_libhal_header_files(${PROJECT_NAME})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_libstd_header_files(${PROJECT_NAME})
target_include_libio_header_files(${PROJECT_NAME})

target_enable_iwyu(${PROJECT_NAME})