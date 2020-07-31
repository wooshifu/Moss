project(ObjArch C ASM)


aux_source_directory(${ObjArch_SOURCE_DIR}/${MossArch} ObjArchSrc)
add_library(${PROJECT_NAME}
        OBJECT
        ${ObjArchSrc}
        ${ObjArch_SOURCE_DIR}/${MossArch}/boot.S)
target_include_kernel_header_files(${PROJECT_NAME})


add_executable(${KernelName}
        $<TARGET_OBJECTS:ObjArch>
        $<TARGET_OBJECTS:ObjKernel>
        $<TARGET_OBJECTS:ObjLibC>
        $<TARGET_OBJECTS:ObjBoardRaspberryPi3>)
target_link_options(${KernelName} PRIVATE -T ${ObjArch_SOURCE_DIR}/${MossArch}/kernel.ld)
target_link_libraries(${KernelName} PRIVATE gcc)


add_custom_command(TARGET ${KernelName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo making Kernel7.img, wait a moment
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_OBJCOPY} ${KernelName} -O binary Kernel7.img)
