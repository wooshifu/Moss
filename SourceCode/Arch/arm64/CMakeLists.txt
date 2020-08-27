project(ObjArch C ASM)


find_asm_source_files(ASM_SOURCE_FILES ${ObjArch_SOURCE_DIR}/src)
aux_source_directory(${ObjArch_SOURCE_DIR}/src ObjArchSrc)
add_library(${PROJECT_NAME}
        OBJECT
        ${ObjArchSrc}
        ${ASM_SOURCE_FILES})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_libc_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})
target_include_raspberry_pi3_header_files(${PROJECT_NAME})

# library for unit tests
add_library(ObjArchUnitTests
        OBJECT
        ${ObjArchSrc}
        ${ASM_SOURCE_FILES})
target_compile_definitions(ObjArchUnitTests PRIVATE "-DENV_KERNEL_UNIT_TESTS")
target_include_kernel_header_files(ObjArchUnitTests)
target_include_libc_header_files(ObjArchUnitTests)
target_include_hal_header_files(ObjArchUnitTests)
target_include_raspberry_pi3_header_files(ObjArchUnitTests)
# library for unit tests


add_executable(${KernelName}
        $<TARGET_OBJECTS:ObjArch>
        $<TARGET_OBJECTS:ObjKernel>
        $<TARGET_OBJECTS:ObjLibC>
        $<TARGET_OBJECTS:ObjBoardRaspberryPi3>)
target_link_options(${KernelName} PRIVATE -T ${ObjArch_SOURCE_DIR}/src/kernel.ld)
target_link_libraries(${KernelName} PRIVATE gcc)


add_custom_command(TARGET ${KernelName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo making Kernel8.img, wait a moment
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_OBJCOPY} ${KernelName} -O binary Kernel8.img)

add_custom_target(RunQemuRaspi3
        COMMAND qemu-system-aarch64 -M raspi3 -kernel Kernel.elf -nographic -serial mon:stdio
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_dependencies(RunQemuRaspi3 ${KernelName})

