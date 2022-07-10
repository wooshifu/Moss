project(ObjLibStd LANGUAGES CXX DESCRIPTION "stdlib")

aux_source_directory(${ObjLibStd_SOURCE_DIR}/src ObjLibStdSrc)

add_library(${PROJECT_NAME}
        OBJECT
        ${ObjLibStdSrc})
set_source_files_properties(${ObjLibStd_SOURCE_DIR}/src/printf.cc PROPERTIES COMPILE_FLAGS "-Wno-gnu-case-range")
target_include_libhal_header_files(${PROJECT_NAME})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_libstd_header_files(${PROJECT_NAME})
target_include_libio_header_files(${PROJECT_NAME})


target_enable_iwyu(${PROJECT_NAME})
