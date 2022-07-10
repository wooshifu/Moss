project(ObjLibHAL CXX)

aux_source_directory(${ObjLibHAL_SOURCE_DIR}/src LibHALSrc)

add_library(${PROJECT_NAME}
        OBJECT
        ${LibHALSrc})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_libstd_header_files(${PROJECT_NAME})
target_include_libhal_header_files(${PROJECT_NAME})

target_enable_iwyu(${PROJECT_NAME})
