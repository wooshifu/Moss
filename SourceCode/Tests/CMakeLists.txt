PROJECT(ObjUnitTests CXX ASM)


aux_source_directory(${ObjUnitTests_SOURCE_DIR}/src UnitTestsSrc)
add_library(${PROJECT_NAME}
        OBJECT
        ${UnitTestsSrc})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_tests_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})
