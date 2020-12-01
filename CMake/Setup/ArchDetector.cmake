# read .arch file from board dir
macro(read_arch_from_arch_file OUT_VAR)
    if (NOT EXISTS ${MOSS_SOURCE_CODE_DIR}/Board/${BOARD}/.arch)
        message(FATAL_ERROR "arch .arch not found in ${MOSS_SOURCE_CODE_DIR}/Board/${BOARD}")
    endif ()

    file(STRINGS ${MOSS_SOURCE_CODE_DIR}/Board/${BOARD}/.arch LINES)
    foreach (LINE IN LISTS LINES)
        # "#" started line is comment line, remove it
        string(REGEX REPLACE "\#.*" "" STRIPPED_VALUE "${LINE}")
        if (STRIPPED_VALUE)
            set(${OUT_VAR} "${STRIPPED_VALUE}")
            break()
        endif ()
    endforeach ()
endmacro()

macro(setup_arch OUT_VAR)
    list_subdirs(SUPPORTED_ARCHS ${MOSS_SOURCE_CODE_DIR}/Arch)

    foreach (SUPPORTED_ARCH ${SUPPORTED_ARCHS})
        message(STATUS "detected supported arch list: ${SUPPORTED_ARCHS}")
    endforeach ()

    read_arch_from_arch_file(${OUT_VAR})

    list(FIND SUPPORTED_ARCHS ${${OUT_VAR}} INDEX)
    if (NOT INDEX EQUAL -1)
        message(STATUS "finally, found arch \"${${OUT_VAR}}\"")
    else ()
        message(FATAL_ERROR "not supported arch: ${${OUT_VAR}}. this is all the supported arch: ${SUPPORTED_ARCHS}")
    endif ()

endmacro()
