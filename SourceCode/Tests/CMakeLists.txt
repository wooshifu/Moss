PROJECT(ObjUnitTests C ASM)


aux_source_directory(${ObjUnitTests_SOURCE_DIR}/src UnitTestsSrc)
add_library(${PROJECT_NAME}
        OBJECT
        ${UnitTestsSrc})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_libc_header_files(${PROJECT_NAME})
target_include_tests_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})


set(ArchDir ${MOSS_SOURCE_CODE_DIR}/Arch/${MossArch}/src)
set(KernelUnitTestsElf KernelUnitTests.elf)
add_executable(${KernelUnitTestsElf}
        $<TARGET_OBJECTS:ObjArchUnitTests>
        $<TARGET_OBJECTS:ObjUnitTests>
        $<TARGET_OBJECTS:ObjKernel>
        $<TARGET_OBJECTS:ObjLibC>
        $<TARGET_OBJECTS:ObjBoardRaspberryPi3>)
set_target_properties(${KernelUnitTestsElf} PROPERTIES LINK_FLAGS "-Xlinker -T ${ArchDir}/kernel.ld -Xlinker -Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/KernelUnitTests.map")
target_link_libraries(${KernelUnitTestsElf} PRIVATE gcc)
