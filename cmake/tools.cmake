# *****************************************************************************
# Name:        tools.cmake
# Library:     ETKSQLite3
# Purpose:     Functions used into root CMakeLists.txt
# Author:      Stéphane Château
# Modified by:
# Created:     2026/02/10
# Copyright:   © Stéphane Château
# Licence:     wxWindows licence
# *****************************************************************************

# =============================================================================
# Custom build step reusableAPI for <files>.ws3
# =============================================================================
function(ETKSQLite3_generate_ws3 ws3_file)
    get_filename_component(base ${ws3_file} NAME_WE)

    #==================================================
    # Compute and create generated ws3 directory
    set(GENERATED_WS3_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated_ws3")
    file(MAKE_DIRECTORY ${GENERATED_WS3_DIR})

    #==================================================
    # Compute 2 generated files path
    set(out_cpp "${GENERATED_WS3_DIR}/${base}.cpp")
    set(out_h  "${GENERATED_WS3_DIR}/${base}.h")

    #====================================================
    # Use Lua executable provided by ETKSQLite3 package
    if(NOT DEFINED ETKSQLite3_LUA_EXECUTABLE)
        message(FATAL_ERROR "ETKSQLite3_LUA_EXECUTABLE not defined. Make sure you called find_package(ETKSQLite3).")
    endif()
    if(NOT EXISTS "${ETKSQLite3_LUA_EXECUTABLE}")
        message(FATAL_ERROR "Lua executable not found at ${ETKSQLite3_LUA_EXECUTABLE}")
    endif()

    #==================================================
    # Use Lua script provided by ETKSQLite3 package
    if(NOT DEFINED ETKSQLite3_SCRIPTS_DIR)
        message(FATAL_ERROR "ETKSQLite3_SCRIPTS_DIR not defined. Make sure you called find_package(ETKSQLite3).")
    endif()
    if(NOT EXISTS "${ETKSQLite3_SCRIPTS_DIR}")
        message(FATAL_ERROR "Lua scripts folder not found at ${ETKSQLite3_SCRIPTS_DIR}")
    endif()

    # Custom command to generate files
    add_custom_command(OUTPUT  "${out_cpp}" "${out_h}"
                       COMMAND "${ETKSQLite3_LUA_EXECUTABLE}"
                               "${ETKSQLite3_SCRIPTS_DIR}/compile_ws3.lua"
                               "${ws3_file}" qt "${out_h}" "${out_cpp}"
                       DEPENDS "${ws3_file}" "${ETKSQLite3_SCRIPTS_DIR}/compile_ws3.lua"
                       COMMENT "Compiling ws3 database ${ws3_file} -> ${out_cpp}, ${out_h}"
                       VERBATIM
    )

    # Indicate the files are generated
    set_source_files_properties("${out_cpp}" "${out_h}" PROPERTIES GENERATED TRUE)

    # Expose generated files to parent scope (append to existing lists)
    if(NOT DEFINED GENERATED_WS3_FILES)
        set(GENERATED_WS3_FILES "")
    endif()
    if(NOT DEFINED GENERATED_WS3_SOURCES)
        set(GENERATED_WS3_SOURCES "")
    endif()
    if(NOT DEFINED GENERATED_WS3_HEADERS)
        set(GENERATED_WS3_HEADERS "")
    endif()
    
    list(APPEND GENERATED_WS3_FILES   "${ws3_file}")
    list(APPEND GENERATED_WS3_SOURCES "${out_cpp}")
    list(APPEND GENERATED_WS3_HEADERS "${out_h}")

    set(GENERATED_WS3_FILES   ${GENERATED_WS3_FILES}   PARENT_SCOPE)
    set(GENERATED_WS3_SOURCES ${GENERATED_WS3_SOURCES} PARENT_SCOPE)
    set(GENERATED_WS3_HEADERS ${GENERATED_WS3_HEADERS} PARENT_SCOPE)
    set(GENERATED_WS3_DIR     ${GENERATED_WS3_DIR}     PARENT_SCOPE)
endfunction()
