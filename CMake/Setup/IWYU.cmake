find_program(OUT_path_to_iwyu NAMES include-what-you-use iwyu)

if (OPTION_DISABLE_IWYU)
    function(target_enable_iwyu IN_target)
    endfunction()

else ()
    if (NOT OUT_path_to_iwyu)
        message(FATAL_ERROR "could not find the program include-what-you-use")
    endif ()

    if (OPTION_IWYU_UPDATE_COMMENTS)
        set(iwyu_update_comments "-Xiwyu;--update_comments;")
    endif ()
    set(__iwyu_options -Xiwyu;--cxx17ns;-Xiwyu;--no_default_mappings;-Xiwyu;--no_fwd_decls;${iwyu_update_comments})

    function(target_enable_iwyu IN_target)
        set_target_properties(${IN_target} PROPERTIES CXX_INCLUDE_WHAT_YOU_USE "${OUT_path_to_iwyu};${__iwyu_options}")
    endfunction()


    find_package(Python3 COMPONENTS Interpreter)
    if (NOT Python3_Interpreter_FOUND)
        message(FATAL_ERROR "python3 is required")
    endif ()
    message(STATUS "found python3: ${Python3_EXECUTABLE}")

    if (NOT EXISTS ${MOSS_SCRIPTS_DIR}/IWYU/include-what-you-use)
        message(FATAL_ERROR "include-what-you-use dir could not be found, use `git pull --recurse-submodules` to pull submodules")
    endif ()

    if (OPTION_IWYU_UPDATE_COMMENTS)
        set(iwyu_update_comments_fix "--update_comments")
    endif ()
    add_custom_target(iwyu_fix_includes
            COMMENT "iwyu fixing includes, wait a moment"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
            COMMAND ${Python3_EXECUTABLE} ${MOSS_SCRIPTS_DIR}/IWYU/include-what-you-use/iwyu_tool.py -p ${CMAKE_BINARY_DIR} -- ${__iwyu_options} > ${CMAKE_BINARY_DIR}/.iwyu_fix_includes || (exit 0)
            COMMAND ${Python3_EXECUTABLE} ${MOSS_SCRIPTS_DIR}/IWYU/include-what-you-use/fix_includes.py --comments ${iwyu_update_comments_fix} < ${CMAKE_BINARY_DIR}/.iwyu_fix_includes || (exit 0)
            )
endif ()
