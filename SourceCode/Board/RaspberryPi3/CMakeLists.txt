project(ObjBoardRaspberryPi3 CXX ASM)

aux_source_directory(${ObjBoardRaspberryPi3_SOURCE_DIR}/src ObjBoardRaspberryPi3Src)
add_library(${PROJECT_NAME}
        OBJECT
        ${ObjBoardRaspberryPi3Src}
        ${ASM_SOURCE_FILES})

target_include_raspberry_pi3_header_files(${PROJECT_NAME})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})
target_include_kernel_header_files(${PROJECT_NAME})

set(COMMON_KERNEL_OBJECTS
        $<TARGET_OBJECTS:ObjKernel>
        $<TARGET_OBJECTS:ObjLibCXX>
        $<TARGET_OBJECTS:ObjBoardRaspberryPi3>
        )

############ start link kernel ############
add_executable(${KernelName}
        $<TARGET_OBJECTS:ObjArch>
        ${COMMON_KERNEL_OBJECTS})
target_link_options(${KernelName} PRIVATE "LINKER:SHELL:-T ${ObjArch_SOURCE_DIR}/src/kernel.ld --Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/KernelArm64.map")

add_custom_command(TARGET ${KernelName} POST_BUILD
        COMMENT "making Kernel8.img, wait a moment"
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_OBJCOPY} ${KernelName} -O binary Kernel8.img)

add_custom_target(run
        COMMENT "run kernel"
        COMMAND qemu-system-aarch64 -M raspi3 -kernel Kernel.elf -nographic -serial mon:stdio
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_dependencies(run ${KernelName})
############ end link kernel ############


############ start link kernel unit tests ############
set(KernelUnitTestsElf KernelUnitTests.elf)
add_executable(${KernelUnitTestsElf}
        $<TARGET_OBJECTS:ObjArchUnitTests>
        $<TARGET_OBJECTS:ObjUnitTests>
        ${COMMON_KERNEL_OBJECTS})
target_link_options(${KernelUnitTestsElf} PRIVATE "LINKER:SHELL:-T ${ObjArch_SOURCE_DIR}/src/kernel.ld --Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/KernelUnitTestsArm64.map")

add_custom_target(test
        COMMENT "run tests"
        COMMAND qemu-system-aarch64 -M raspi3 -kernel ${KernelUnitTestsElf} -nographic -serial mon:stdio
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_dependencies(test ${KernelUnitTestsElf})
############ end link kernel unit tests ############
