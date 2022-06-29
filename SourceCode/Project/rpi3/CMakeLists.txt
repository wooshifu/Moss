project(ObjProjectRpi3 CXX ASM)

aux_source_directory(${ObjProjectRpi3_SOURCE_DIR}/src ObjProjectRpi3Src)
add_library(${PROJECT_NAME}
        OBJECT
        ${ObjProjectRpi3Src})

target_include_rpi3_header_files(${PROJECT_NAME})
target_include_libcxx_header_files(${PROJECT_NAME})
target_include_libstd_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})
target_include_kernel_header_files(${PROJECT_NAME})
target_include_specific_arch_header_files(${PROJECT_NAME} aarch64)
target_enable_iwyu(${PROJECT_NAME})

add_subdirectory(${MOSS_SOURCE_CODE_DIR}/Platform/bcm28xx ${CMAKE_CURRENT_BINARY_DIR}/Platform/bcm28xx)


set(COMMON_KERNEL_OBJECTS
        $<TARGET_OBJECTS:ObjKernel>
        $<TARGET_OBJECTS:ObjLibCXX>
        $<TARGET_OBJECTS:ObjLibStd>
        $<TARGET_OBJECTS:ObjLibIO>
        $<TARGET_OBJECTS:ObjHAL>
        $<TARGET_OBJECTS:ObjProjectRpi3>
        $<TARGET_OBJECTS:ObjPlatBCM28xx>
        )

############ start link kernel ############
# todo: enable iwyu check to all source files from ${KernelName}
add_executable(${KernelName}
        $<TARGET_OBJECTS:ObjArch>
        ${COMMON_KERNEL_OBJECTS})
target_link_options(${KernelName} PRIVATE "LINKER:-L${MOSS_SOURCE_CODE_DIR}/Kernel,-T,${ObjArch_SOURCE_DIR}/src/kernel.lds,--Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/KernelArm64.map")

add_custom_command(TARGET ${KernelName} POST_BUILD
        COMMENT "making kernel8.img, wait a moment"
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_OBJCOPY} ${KernelName} -O binary kernel8.img)

add_custom_target(run
        COMMENT "run kernel"
        USES_TERMINAL
        COMMAND qemu-system-aarch64 -M raspi3b -kernel kernel.elf -semihosting -serial null -serial mon:stdio -nographic
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_dependencies(run ${KernelName})
############ end link kernel ############
