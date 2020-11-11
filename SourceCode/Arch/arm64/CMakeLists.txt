project(ObjArch CXX ASM)


find_asm_source_files(ASM_SOURCE_FILES ${ObjArch_SOURCE_DIR}/src)
aux_source_directory(${ObjArch_SOURCE_DIR}/src ObjArchSrc)
add_library(${PROJECT_NAME}
        OBJECT
        ${ObjArchSrc}
        ${ASM_SOURCE_FILES})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_libc_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})
target_include_arch_header_files(${PROJECT_NAME})

# library for unit tests
add_library(ObjArchUnitTests
        OBJECT
        ${ObjArchSrc}
        ${ASM_SOURCE_FILES})
target_compile_definitions(ObjArchUnitTests PRIVATE "-DENV_KERNEL_UNIT_TESTS")
target_include_kernel_header_files(ObjArchUnitTests)
target_include_libc_header_files(ObjArchUnitTests)
target_include_hal_header_files(ObjArchUnitTests)
target_include_arch_header_files(ObjArchUnitTests)
# library for unit tests
