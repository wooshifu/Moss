project(ObjArch C CXX ASM)


find_asm_source_files(ASM_SOURCE_FILES ${ObjArch_SOURCE_DIR}/src)
aux_source_directory(${ObjArch_SOURCE_DIR}/src ObjArchSrc)
add_library(${PROJECT_NAME}
        OBJECT
        ${ObjArchSrc}
        ${ASM_SOURCE_FILES})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_libstdcxx_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})
target_include_specific_arch_header_files(${PROJECT_NAME} aarch64)
target_include_arch_header_files(${PROJECT_NAME})

target_enable_iwyu(${PROJECT_NAME})

# library for unit tests
set(ObjArchUnitTests ObjArchUnitTests)
add_library(${ObjArchUnitTests}
        OBJECT
        ${ObjArchSrc}
        ${ASM_SOURCE_FILES})
target_compile_definitions(${ObjArchUnitTests} PRIVATE ENV_KERNEL_UNIT_TESTS=1)
target_include_kernel_header_files(${ObjArchUnitTests})
target_include_libcxx_header_files(${ObjArchUnitTests})
target_include_libstdcxx_header_files(${ObjArchUnitTests})
target_include_hal_header_files(${ObjArchUnitTests})
target_include_specific_arch_header_files(${ObjArchUnitTests} aarch64)
target_include_arch_header_files(${ObjArchUnitTests})

target_enable_iwyu(${ObjArchUnitTests})
# library for unit tests
