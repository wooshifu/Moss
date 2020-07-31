project(ObjBoardRaspberryPi3 C ASM)

aux_source_directory(${ObjBoardRaspberryPi3_SOURCE_DIR}/src ObjBoardRaspberryPi3Src)

add_library(${PROJECT_NAME}
        OBJECT
        ${ObjBoardRaspberryPi3Src})

target_include_libc_header_files(${PROJECT_NAME})
target_include_raspberry_pi3_header_files(${PROJECT_NAME})
target_include_kernel_header_files(${PROJECT_NAME})
