project(ObjBoardRaspberryPi3 CXX ASM)

aux_source_directory(${ObjBoardRaspberryPi3_SOURCE_DIR}/src ObjBoardRaspberryPi3Src)
add_library(${PROJECT_NAME}
        OBJECT
        ${ObjBoardRaspberryPi3Src})

target_include_rpi3_header_files(${PROJECT_NAME})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_libstd_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_specific_arch_header_files(${PROJECT_NAME} aarch64)
target_enable_iwyu(${PROJECT_NAME})


set(COMMON_KERNEL_OBJECTS
        $<TARGET_OBJECTS:ObjKernel>
        $<TARGET_OBJECTS:ObjLibCXX>
        $<TARGET_OBJECTS:ObjHAL>
        $<TARGET_OBJECTS:ObjBoardRaspberryPi3>
        )

############ start link kernel ############
# todo: enable iwyu check to all source files from ${KernelName}
add_executable(${KernelName}
        $<TARGET_OBJECTS:ObjArch>
        ${COMMON_KERNEL_OBJECTS})
target_link_options(${KernelName} PRIVATE "LINKER:-T,${ObjArch_SOURCE_DIR}/src/kernel.ld,--Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/KernelArm64.map")

#todo: disabled because of "LLVM ERROR: out of memory" when running llvm-objcopy
#add_custom_command(TARGET ${KernelName} POST_BUILD
#        COMMENT "making kernel8.img, wait a moment"
#        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#        COMMAND ${CMAKE_OBJCOPY} ${KernelName} -O binary kernel8.img)

add_custom_target(run
        COMMENT "run kernel"
        USES_TERMINAL
        COMMAND qemu-system-aarch64 -M raspi3b -kernel kernel.elf -semihosting -serial null -serial mon:stdio -nographic -d in_asm
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_dependencies(run ${KernelName})
############ end link kernel ############
