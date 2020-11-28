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


add_executable(${KernelName}
        $<TARGET_OBJECTS:ObjArch>
        $<TARGET_OBJECTS:ObjKernel>
        $<TARGET_OBJECTS:ObjLibCXX>
        $<TARGET_OBJECTS:ObjBoardRaspberryPi3>)
# following line also works
#target_link_options(${KernelName} PRIVATE -T ${ObjArch_SOURCE_DIR}/src/kernel.ld -Xlinker -Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/KernelArm64.map)
set_target_properties(${KernelName} PROPERTIES LINK_FLAGS "-Xlinker -T ${ObjArch_SOURCE_DIR}/src/kernel.ld -Xlinker -Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/KernelArm64.map")
target_link_libraries(${KernelName} PRIVATE gcc)


add_custom_command(TARGET ${KernelName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo making Kernel8.img, wait a moment
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_OBJCOPY} ${KernelName} -O binary Kernel8.img)

add_custom_target(RunQemuRaspi3
        COMMAND qemu-system-aarch64 -M raspi3 -kernel Kernel.elf -nographic -serial mon:stdio
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_dependencies(RunQemuRaspi3 ${KernelName})

