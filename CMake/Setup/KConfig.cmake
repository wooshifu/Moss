function(setup_kconfig)
    find_package(Python3 COMPONENTS Interpreter)
    if (NOT Python3_Interpreter_FOUND)
        message(FATAL_ERROR "python3 is required")
    endif ()
    message(STATUS "found python3: ${Python3_EXECUTABLE}")

    set(kconfiglib_path ${Moss_SOURCE_DIR}/KconfigLib/Kconfiglib)
    if (NOT EXISTS ${Moss_SOURCE_DIR}/.config)
        message(STATUS "${Moss_SOURCE_DIR}/.config not found. generating default .config file by running ${Python3_EXECUTABLE} ${Moss_SOURCE_DIR}/KConfig/alldefconfig.py")
        execute_process(COMMAND ${Python3_EXECUTABLE}
                ${kconfiglib_path}/alldefconfig.py
                WORKING_DIRECTORY ${Moss_SOURCE_DIR}
                TIMEOUT 5)
    endif ()

    set(config_dir ${Moss_SOURCE_DIR}/SourceCode/Config)
    set(genconfig_command ${kconfiglib_path}/genconfig.py
            --header-path ${config_dir}/include/kconfig.hh
            --file-list ${config_dir}/KConfigFileList.txt
            --env-list ${config_dir}/KConfigEnvList.txt)
    message(STATUS "generating config header files by running ${Python3_EXECUTABLE} ${genconfig_command}")
    execute_process(COMMAND ${Python3_EXECUTABLE}
            ${genconfig_command}
            WORKING_DIRECTORY ${Moss_SOURCE_DIR}
            TIMEOUT 5)

    add_custom_target(alldefconfig.py
            COMMENT "generate all default config"
            COMMAND ${Python3_EXECUTABLE} ${kconfiglib_path}/alldefconfig.py
            WORKING_DIRECTORY ${Moss_SOURCE_DIR})
    add_custom_target(genconfig.py
            COMMENT "generate header files from config"
            COMMAND ${Python3_EXECUTABLE} ${genconfig_command}
            WORKING_DIRECTORY ${Moss_SOURCE_DIR})
endfunction()
