PROJECT(ObjUnitTests CXX ASM)


aux_source_directory(${ObjUnitTests_SOURCE_DIR}/src UnitTestsSrc)
add_library(${PROJECT_NAME}
        OBJECT
        ${UnitTestsSrc})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_tests_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})


set(ArchDir ${MOSS_SOURCE_CODE_DIR}/Arch/${MossArch}/src)
set(KernelUnitTestsElf KernelUnitTests.elf)
add_executable(${KernelUnitTestsElf}
        $<TARGET_OBJECTS:ObjArchUnitTests>
        $<TARGET_OBJECTS:ObjUnitTests>
        $<TARGET_OBJECTS:ObjKernel>
        $<TARGET_OBJECTS:ObjLibCXX>
        $<TARGET_OBJECTS:ObjBoardRaspberryPi3>)
set_target_properties(${KernelUnitTestsElf} PROPERTIES LINK_FLAGS "-Xlinker -T ${ArchDir}/kernel.ld -Xlinker -Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/KernelUnitTests.map")
target_link_libraries(${KernelUnitTestsElf} PRIVATE gcc)

add_custom_target(test
        COMMAND qemu-system-aarch64 -M raspi3 -kernel ${KernelUnitTestsElf} -nographic -serial mon:stdio
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_dependencies(test ${KernelUnitTestsElf})
