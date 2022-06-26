project(ObjUnitTests CXX ASM)

function(find_test_cases)
    FILE(GLOB TestHeaderFiles LIST_DIRECTORIES false ${ObjUnitTests_SOURCE_DIR}/include/test_*.hh)
    message(STATUS "TestHeaderFiles: ${TestHeaderFiles}")
    foreach (TestHeaderFile ${TestHeaderFiles})
        get_filename_component(TestFile ${TestHeaderFile} NAME)
        message(STATUS "found test file: ${TestFile}")
        list(APPEND TestFileName ${TestFile})

        file(STRINGS ${TestHeaderFile} LINES)
        foreach (LINE ${LINES})
            if (LINE MATCHES "[ \t\r\n]*TEST\\(\([A-Za-z0-9_]+\)\\).*")
                message(STATUS "found test case: ${CMAKE_MATCH_1}")
                list(APPEND TestCaseName ${CMAKE_MATCH_1})
            endif ()
        endforeach ()
    endforeach ()
    string(APPEND CasesHppFileContent "/* auto generated code, do not modify!!! */\n\n")
    string(APPEND CasesHppFileContent "#pragma once\n\n")
    string(APPEND CasesHppFileContent "#include \"tests/test_lib.hh\"\n\n")
    foreach (FileName ${TestFileName})
        string(APPEND CasesHppFileContent "#include \"${FileName}\"\n")
    endforeach ()
    string(APPEND CasesHppFileContent "using test_case_func        = void (*)()\;\n\n")
    string(APPEND CasesHppFileContent "test_case_func test_cases[] = {\n")
    string(APPEND CasesHppFileContent "    // clang-format off\n")
    foreach (FileCase ${TestCaseName})
        string(APPEND CasesHppFileContent "    ${FileCase},\n")
    endforeach ()
    string(APPEND CasesHppFileContent "    // clang-format on\n")
    string(APPEND CasesHppFileContent "}\;\n")
    file(WRITE ${ObjUnitTests_SOURCE_DIR}/include/cases.hh ${CasesHppFileContent})
    message(STATUS "generated test cases have been written to ${ObjUnitTests_SOURCE_DIR}/include/cases.hh")
endfunction()
find_test_cases()

aux_source_directory(${ObjUnitTests_SOURCE_DIR}/src UnitTestsSrc)
add_library(${PROJECT_NAME}
        OBJECT
        ${UnitTestsSrc})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_libstd_header_files(${PROJECT_NAME})
target_include_tests_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})

target_enable_iwyu(${PROJECT_NAME})


############ start link kernel unit tests ############
set(COMMON_KERNEL_OBJECTS
        $<TARGET_OBJECTS:ObjKernel>
        $<TARGET_OBJECTS:ObjLibCXX>
        $<TARGET_OBJECTS:ObjLibStd>
        $<TARGET_OBJECTS:ObjLibIO>
        $<TARGET_OBJECTS:ObjHAL>
        $<TARGET_OBJECTS:ObjBoardRaspberryPi3>
        $<TARGET_OBJECTS:ObjPlatBCM28xx>
        )
set(KernelUnitTestsElf KernelUnitTests.elf)
add_executable(${KernelUnitTestsElf}
        $<TARGET_OBJECTS:ObjArchUnitTests>
        $<TARGET_OBJECTS:ObjUnitTests>
        ${COMMON_KERNEL_OBJECTS})
target_link_options(${KernelUnitTestsElf} PRIVATE "LINKER:-L${MOSS_SOURCE_CODE_DIR}/Kernel,-T,${ObjArch_SOURCE_DIR}/src/kernel.lds,--Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/KernelUnitTestsArm64.map")

add_custom_target(test
        COMMENT "run tests"
        COMMAND qemu-system-aarch64 -M raspi3 -kernel ${KernelUnitTestsElf} -nographic -serial mon:stdio
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_dependencies(test ${KernelUnitTestsElf})
############ end link kernel unit tests ############
