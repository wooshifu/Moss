macro(read_board_from_board_file OUT_VAR)
    if (NOT EXISTS ${Moss_SOURCE_DIR}/.board)
        message(FATAL_ERROR "file .board not found in ${Moss_SOURCE_DIR}")
    endif ()

    file(STRINGS ${Moss_SOURCE_DIR}/.board LINES)
    foreach (LINE IN LISTS LINES)
        # "#" started line is comment line, remove it
        string(REGEX REPLACE "\#.*" "" STRIPPED_VALUE "${LINE}")
        if (STRIPPED_VALUE)
            set(${OUT_VAR} "${STRIPPED_VALUE}")
            break()
        endif ()
    endforeach ()
endmacro()

macro(list_subdirs result current_dir)
    FILE(GLOB children RELATIVE ${current_dir} ${current_dir}/*)
    SET(temp_dirlist "")
    foreach (child ${children})
        if (IS_DIRECTORY ${current_dir}/${child})
            LIST(APPEND temp_dirlist ${child})
        endif ()
    endforeach ()
    set(${result} ${temp_dirlist})
endmacro()

# 1. check variable BOARD is defined via cmake command line as parameter, use it if it's defined
# 2. read file content from .board as BOARD variable
macro(setup_board)
    list_subdirs(SUPPORTED_BOARDS ${MOSS_SOURCE_CODE_DIR}/Board)
    foreach (SUPPORTED_BOARD ${SUPPORTED_BOARDS})
        message(STATUS "detected supported board list: ${SUPPORTED_BOARD}")
    endforeach ()

    # read BOARD from cmake command line parameter
    if (DEFINED BOARD)
        message(STATUS "DEFINED BOARD: ${BOARD}")
    else ()
        read_board_from_board_file(BOARD)
        message(STATUS "READ BOARD FROM FILE: ${BOARD}")
    endif ()

    list(FIND SUPPORTED_BOARDS ${BOARD} INDEX)
    if (NOT INDEX EQUAL -1)
        message(STATUS "finally, found board \"${BOARD}\"")
    else ()
        message(FATAL_ERROR "not supported board: ${BOARD}. this is all the supported boards: ${SUPPORTED_BOARDS}")
    endif ()

    set(BOARD ${BOARD})
endmacro()
