# read arch.cmake file from board dir
function(read_arch_from_arch_cmake_file IN_board OUT_arch)
    set(arch_cmake_file ${MOSS_SOURCE_CODE_DIR}/Board/${IN_board}/arch.cmake)
    if (NOT EXISTS ${arch_cmake_file})
        message(FATAL_ERROR "file ${arch_cmake_file} not found")
    endif ()

    message(STATUS "by convention, function(get_board_arch OUT_board_arch) should be defined in ${arch_cmake_file}")
    include(${arch_cmake_file})
    get_board_arch(BOARD_ARCH)
    message(STATUS "board arch: ${BOARD_ARCH}")

    set(${OUT_arch} "${BOARD_ARCH}" PARENT_SCOPE)
endfunction()

function(setup_arch IN_board OUT_arch)
    list_subdirs(supported_archs ${MOSS_SOURCE_CODE_DIR}/Arch)

    foreach (SUPPORTED_ARCH ${supported_archs})
        message(STATUS "detected supported arch list: ${supported_archs}")
    endforeach ()

    read_arch_from_arch_cmake_file(${IN_board} arch)
    set(${OUT_arch} ${arch} PARENT_SCOPE)

    list(FIND supported_archs ${arch} index)
    if (NOT index EQUAL -1)
        message(STATUS "finally, found arch \"${arch}\"")
    else ()
        message(FATAL_ERROR "not supported arch: ${arch}. this is all the supported arch: ${supported_archs}")
    endif ()
endfunction()
