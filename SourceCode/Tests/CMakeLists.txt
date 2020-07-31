PROJECT(ObjUnitTests C ASM)


aux_source_directory(${ObjUnitTests_SOURCE_DIR}/src UnitTestsSrc)
add_library(${PROJECT_NAME}
        OBJECT
        ${UnitTestsSrc})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_libc_header_files(${PROJECT_NAME})
target_include_tests_header_files(${PROJECT_NAME})
target_include_raspberry_pi3_header_files(${PROJECT_NAME})


set(ArchDir ${Moss_SOURCE_DIR}/SourceCode/Arch/${MossArch})
aux_source_directory(${ArchDir} ArchSrc)
add_library(ObjArchUnitTests
        OBJECT
        ${ArchSrc}
        ${ArchDir}/boot.S)
target_compile_definitions(ObjArchUnitTests PRIVATE "-DENV_KERNEL_UNIT_TESTS")
target_include_kernel_header_files(ObjArchUnitTests)


set(KernelUnitTestsElf KernelUnitTests.elf)
add_executable(${KernelUnitTestsElf}
        $<TARGET_OBJECTS:ObjArchUnitTests>
        $<TARGET_OBJECTS:ObjUnitTests>
        $<TARGET_OBJECTS:ObjKernel>
        $<TARGET_OBJECTS:ObjLibC>
        $<TARGET_OBJECTS:ObjBoardRaspberryPi3>)
target_link_options(${KernelUnitTestsElf} PRIVATE -T ${ArchDir}/kernel.ld)
target_link_libraries(${KernelUnitTestsElf} PRIVATE gcc)
