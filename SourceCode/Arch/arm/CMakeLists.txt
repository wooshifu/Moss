project(ObjArch CXX ASM)

find_asm_source_files(ASM_SOURCE_FILES ${ObjArch_SOURCE_DIR}/src)
aux_source_directory(${ObjArch_SOURCE_DIR}/src ObjArchSrc)
add_library(${PROJECT_NAME}
        OBJECT
        ${ObjArchSrc}
        ${ASM_SOURCE_FILES})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_raspberry_pi3_header_files(${PROJECT_NAME})


add_executable(${KernelName}
        $<TARGET_OBJECTS:ObjArch>
        $<TARGET_OBJECTS:ObjKernel>
        $<TARGET_OBJECTS:ObjLibC>
        $<TARGET_OBJECTS:ObjBoardRaspberryPi3>)
set_target_properties(${KernelName} PROPERTIES LINK_FLAGS "-Xlinker -T ${ObjArch_SOURCE_DIR}/src/kernel.ld -Xlinker -Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/KernelArm32.map")
target_link_libraries(${KernelName} PRIVATE gcc)


add_custom_command(TARGET ${KernelName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo making Kernel7.img, wait a moment
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_OBJCOPY} ${KernelName} -O binary Kernel7.img)

add_custom_target(RunQemuRaspi2
        COMMAND qemu-system-arm -M raspi2 -kernel kernel.elf -nographic -serial mon:stdio
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_dependencies(RunQemuRaspi3 ${KernelName})