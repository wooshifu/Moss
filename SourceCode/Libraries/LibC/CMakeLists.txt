PROJECT(ObjLibC CXX ASM)

aux_source_directory(${ObjLibC_SOURCE_DIR}/src LibCSrc)

add_library(${PROJECT_NAME}
        OBJECT
        ${LibCSrc})
target_include_hal_header_files(${PROJECT_NAME})
target_include_libc_header_files(${PROJECT_NAME})
