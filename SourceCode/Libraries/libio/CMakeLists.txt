project(ObjLibIO CXX)

aux_source_directory(${ObjLibIO_SOURCE_DIR}/src LibIOSrc)

add_library(${PROJECT_NAME}
        OBJECT
        ${LibIOSrc})
target_include_libio_header_files(${PROJECT_NAME})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_libstd_header_files(${PROJECT_NAME})
target_include_libhal_header_files(${PROJECT_NAME})

target_enable_iwyu(${PROJECT_NAME})
