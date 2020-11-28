# find all cmake files, and then include the file
file(GLOB CMakeFiles LIST_DIRECTORIES false ${Moss_SOURCE_DIR}/CMake/Setup/*.cmake)
message(STATUS "found CmakeFiles: ${CMakeFiles}")
foreach (CMakeFile ${CMakeFiles})
    message(STATUS "include ${CMakeFile}")
    include(${CMakeFile})
endforeach ()
