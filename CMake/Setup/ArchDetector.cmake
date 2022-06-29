# read arch.cmake file from project dir
function(read_arch_from_arch_cmake_file IN_project OUT_arch)
    set(arch_cmake_file ${MOSS_SOURCE_CODE_DIR}/Project/${IN_project}/arch.cmake)
    if (NOT EXISTS ${arch_cmake_file})
        message(FATAL_ERROR "file ${arch_cmake_file} not found")
    endif ()

    include(${arch_cmake_file})
    message(STATUS "NOTE: by convention, function(get_project_arch OUT_project_arch) should be defined in ${arch_cmake_file}")
    get_project_arch(PROJECT_ARCH)
    message(STATUS "project arch: ${PROJECT_ARCH}")

    set(${OUT_arch} "${PROJECT_ARCH}" PARENT_SCOPE)
endfunction()

function(setup_arch IN_project OUT_arch)
    list_subdirs(supported_archs ${MOSS_SOURCE_CODE_DIR}/Arch)

    foreach (SUPPORTED_ARCH ${supported_archs})
        message(STATUS "detected supported arch list: ${supported_archs}")
    endforeach ()

    read_arch_from_arch_cmake_file(${IN_project} arch)
    set(${OUT_arch} ${arch} PARENT_SCOPE)

    list(FIND supported_archs ${arch} index)
    if (NOT index EQUAL -1)
        message(STATUS "finally, found arch \"${arch}\"")
    else ()
        message(FATAL_ERROR "not supported arch: ${arch}. this is all the supported arch: ${supported_archs}")
    endif ()
endfunction()
