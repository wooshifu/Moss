# find all cmake files, and then include the file
file(GLOB CMakeFiles LIST_DIRECTORIES false ${Moss_SOURCE_DIR}/CMake/Setup/*.cmake)
message(STATUS "found CmakeFiles: ${CMakeFiles}")
foreach (CMakeFile ${CMakeFiles})
    message(STATUS "include ${CMakeFile}")
    include(${CMakeFile})
endforeach ()

message(STATUS "********** setup began **********")

setup_board()
message(STATUS ">>> building board: ${BOARD}")

setup_arch(ARCH)
message(STATUS ">>> building arch: ${ARCH}")

setup_compiler_flags()

message(STATUS "********** setup ended **********")
