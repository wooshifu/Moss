project(ObjArch C CXX ASM)

aux_source_directory(${ObjArch_SOURCE_DIR}/src ObjArchSrc)
add_library(${PROJECT_NAME}
        OBJECT
        ${ObjArchSrc})
set_source_files_properties(${ObjArch_SOURCE_DIR}/src/arm_generic_timer.cc PROPERTIES COMPILE_FLAGS "-Wno-gnu-statement-expression")
target_include_kernel_header_files(${PROJECT_NAME})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_libstd_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})
target_include_specific_arch_header_files(${PROJECT_NAME} aarch64)

target_enable_iwyu(${PROJECT_NAME})
