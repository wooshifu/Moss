project(ObjBoardRaspberryPi3 C ASM)

aux_source_directory(${ObjBoardRaspberryPi3_SOURCE_DIR}/src ObjBoardRaspberryPi3Src)
add_library(${PROJECT_NAME}
        OBJECT
        ${ObjBoardRaspberryPi3Src}
        ${ASM_SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${ObjBoardRaspberryPi3_SOURCE_DIR}/include)
target_include_libc_header_files(${PROJECT_NAME})
target_include_hal_header_files(${PROJECT_NAME})
target_include_kernel_header_files(${PROJECT_NAME})
