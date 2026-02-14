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

# ============================================================
# Helper to find wxWidgets library
# ============================================================
function(ETKSQLite3_find_wxwidgets_auto)
    set(oneValueArgs ROOT_DIR LIB_DIR WX_FORCE_DLL WX_FORCE_LIB)
    set(multiValueArgs)
    cmake_parse_arguments(FWX "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT FWX_ROOT_DIR OR NOT EXISTS "${FWX_ROOT_DIR}")
        if(NOT FWX_ROOT_DIR)
            set(wxWidgets_ROOT_DIR "${FWX_ROOT_DIR}" CACHE PATH "Path to wxWidgets install" FORCE)
        endif()
        message(FATAL_ERROR
            "wxWidgets_ROOT_DIR not set and wxWidgets install was not found next to ETKSQLite3.\n"
            "Expected layout for automatic detection:\n"
            "  ${_ETK_PARENT_DIR}/\n"
            "    ├─ ETKSQLite3/\n"
            "    └─ wxWidgets-x.y.z/\n"
            "        └─ <build_folder>/\n"
            "             └─ install/\n"
            "Or define wxWidgets_ROOT_DIR manually to install wxWidgets folder.")
    endif()

    # ============================================================
    # Decide STATIC or DLL based on BUILD_SHARED_LIBS
    if(FWX_WX_FORCE_DLL AND FWX_WX_FORCE_LIB)
        message(FATAL_ERROR "You cannot set wxWidgets_WX_FORCE_DLL and wxWidgets_WX_FORCE_LIB at the same time!")
    endif()

    if((BUILD_SHARED_LIBS OR FWX_WX_FORCE_DLL) AND NOT FWX_WX_FORCE_LIB)
        if(FWX_WX_FORCE_DLL)
            message(STATUS "Forcing wxWidgets DLL build")
        else()
            message(STATUS "Library is SHARED: allowing wxWidgets DLL build")
        endif()
        set(WX_FORCE_DLL TRUE)
        set(DEFAULT_LIB_SUBDIR "vc_x64_dll")
    else()
        if(FWX_WX_FORCE_LIB)
            message(STATUS "Forcing wxWidgets in static build")
        else()
            message(STATUS "Library is STATIC: allowing wxWidgets static")
        endif()
        set(WX_FORCE_DLL FALSE)
        set(DEFAULT_LIB_SUBDIR "vc_x64_lib")
    endif()

    if(NOT FWX_LIB_DIR)
        set(FWX_LIB_DIR "${FWX_ROOT_DIR}/lib/${DEFAULT_LIB_SUBDIR}")
    endif()

    # ============================================================
    if(WX_FORCE_DLL)
        set(FRAMEWORK_DEFINITIONS WXUSINGDLL)
    else()
        set(FRAMEWORK_DEFINITIONS "")
    endif()

    # --- Find monolithic library wxmsw*.lib dynamically ---
    set(WX_LIBS_RELEASE "")
    set(WX_LIBS_DEBUG "")

    # Liste tous les .lib monolithiques
    file(GLOB WX_ALL_LIBS_RELEASE "${FWX_LIB_DIR}/wxmsw*u.lib")
    file(GLOB WX_ALL_LIBS_DEBUG   "${FWX_LIB_DIR}/wxmsw*ud.lib")

    # Prend un seul fichier correspondant au pattern le plus courant
    # Release
    if(WX_ALL_LIBS_RELEASE)
        list(SORT WX_ALL_LIBS_RELEASE)   # trie par nom pour prendre le plus récent en premier
        list(GET WX_ALL_LIBS_RELEASE 0 WXWIDGETS_LIB_RELEASE)
    endif()

    # Debug
    if(WX_ALL_LIBS_DEBUG)
        list(SORT WX_ALL_LIBS_DEBUG)
        list(GET WX_ALL_LIBS_DEBUG 0 WXWIDGETS_LIB_DEBUG)
    endif()

    message(STATUS "Detected wxWidgets Release lib: ${WXWIDGETS_LIB_RELEASE}")
    message(STATUS "Detected wxWidgets Debug lib: ${WXWIDGETS_LIB_DEBUG}")

    if(WXWIDGETS_LIB_RELEASE OR WXWIDGETS_LIB_DEBUG)
        # === Mode MONOLITHIC ===
        message(STATUS "wxWidgets detected in MONOLITHIC mode")

        # ============================================================
        # Detect if DLL build (check for corresponding .dll)
        set(WX_IS_DLL ${WX_FORCE_DLL})
        if (NOT WX_FORCE_DLL)
            if(WXWIDGETS_LIB_DEBUG)
                get_filename_component(_wx_lib_path ${WXWIDGETS_LIB_DEBUG} DIRECTORY)
                file(GLOB _wx_dlls "${_wx_lib_path}/*.dll")
                if(_wx_dlls)
                    set(WX_IS_DLL TRUE)
                endif()
            endif()
            if(WX_IS_DLL)
                message(STATUS "wxWidgets detected as DLL build")
                set(FRAMEWORK_DEFINITIONS WXUSINGDLL)
            else()
                message(STATUS "wxWidgets detected as STATIC build")
                set(FRAMEWORK_DEFINITIONS "")
            endif()
        endif()

        if(WX_IS_DLL)
            set(WX_DLL_DIR "${FWX_ROOT_DIR}/bin/vc_x64_dll")

            # Separate Debug and Release DLL lists
            set(WX_DLLS_DEBUG "")
            set(WX_DLLS_RELEASE "")

            # --- Release DLLs ---
            if(WXWIDGETS_LIB_RELEASE)
                get_filename_component(release_lib_name ${WXWIDGETS_LIB_RELEASE} NAME_WE)
                # wxmsw33u -> wxmsw33[0-9]*u.*
                string(REGEX REPLACE
                       "([^0-9]+)([0-9]+)(u?)$"
                       "\\1\\2[0-9]*\\3_.*"
                       release_regex
                       "${release_lib_name}")
            endif()

            # --- Debug DLLs ---
            if(WXWIDGETS_LIB_DEBUG)
                get_filename_component(debug_lib_name ${WXWIDGETS_LIB_DEBUG} NAME_WE)
                # wxmsw33ud -> wxmsw33[0-9]*ud
                string(REGEX REPLACE
                       "([^0-9]+)([0-9]+)(u?d)$"
                       "\\1\\2[0-9]*\\3_.*"
                       debug_regex
                       "${debug_lib_name}")
            endif()

            # Liste toutes les DLL wx
            file(GLOB WX_ALL_DLLS "${WX_DLL_DIR}/wx*.dll")

            foreach(dll ${WX_ALL_DLLS})
                get_filename_component(dll_name ${dll} NAME)
                if(release_regex AND dll_name MATCHES "${release_regex}")
                    message(STATUS "Detected wxWidgets Release dynamic library: ${dll}")
                    list(APPEND WX_DLLS_RELEASE "${dll}")
                elseif(debug_regex AND dll_name MATCHES "${debug_regex}")
                    message(STATUS "Detected wxWidgets Debug dynamic library: ${dll}")
                    list(APPEND WX_DLLS_DEBUG "${dll}")
                endif()
            endforeach()

            # Export lists pour le makefile / target post-build
            set(wxWidgets_DLLS_DEBUG   "${WX_DLLS_DEBUG}"   PARENT_SCOPE)
            set(wxWidgets_DLLS_RELEASE "${WX_DLLS_RELEASE}" PARENT_SCOPE)
        endif()

        set(FRAMEWORK_LIBS
            $<$<CONFIG:Debug>:${WXWIDGETS_LIB_DEBUG}>
            $<$<CONFIG:Release>:${WXWIDGETS_LIB_RELEASE}>
            $<$<CONFIG:RelWithDebInfo>:${WXWIDGETS_LIB_RELEASE}>
            $<$<CONFIG:MinSizeRel>:${WXWIDGETS_LIB_RELEASE}>
            gdiplus msimg32 winmm comctl32 uuid oleacc uxtheme rpcrt4 shlwapi version wsock32
        )

        set(FRAMEWORK_INCLUDE_DIRS "${FWX_ROOT_DIR}/include"
                                   $<$<CONFIG:Debug>:${FWX_LIB_DIR}/mswud>
                                   $<$<CONFIG:Release>:${FWX_LIB_DIR}/mswu>
                                   $<$<CONFIG:RelWithDebInfo>:${FWX_LIB_DIR}/mswu>
                                   $<$<CONFIG:MinSizeRel>:${FWX_LIB_DIR}/mswu>)

        set(wxWidgets_ROOT_DIR "${FWX_ROOT_DIR}" CACHE PATH "Path to wxWidgets install" FORCE)
        set(wxWidgets_LIB_DIR  "${FWX_LIB_DIR}" CACHE PATH  "Path to wxWidgets libs"    FORCE)
    else()
        # === Mode MODULES ===
        message(STATUS "wxWidgets monolithic libs not found: falling back to MODULE mode")

        set(wxWidgets_ROOT_DIR "${FWX_ROOT_DIR}" CACHE PATH "Path to wxWidgets install" FORCE)
        set(wxWidgets_LIB_DIR  "${FWX_LIB_DIR}" CACHE PATH  "Path to wxWidgets libs"    FORCE)

        find_package(wxWidgets REQUIRED core base)
        include(${wxWidgets_USE_FILE})

        set(FRAMEWORK_LIBS "${wxWidgets_LIBRARIES}")
        set(FRAMEWORK_INCLUDE_DIRS "${wxWidgets_INCLUDE_DIRS}")
    endif()

    # Create the target IMPORTED only id doesn't exists
    if(NOT TARGET wxWidgets::wxWidgets)
        add_library(wxWidgets::wxWidgets INTERFACE IMPORTED)
    endif()

    # And update properties
    set_target_properties(wxWidgets::wxWidgets PROPERTIES
                          INTERFACE_LINK_LIBRARIES "${FRAMEWORK_LIBS}"
                          INTERFACE_INCLUDE_DIRECTORIES "${FRAMEWORK_INCLUDE_DIRS}"
                          INTERFACE_COMPILE_DEFINITIONS "${FRAMEWORK_DEFINITIONS}"
    )

    # Export variables to parent
    set(wxWidgets_FRAMEWORK_LIBS "${FRAMEWORK_LIBS}" PARENT_SCOPE)
    set(wxWidgets_FRAMEWORK_INCLUDE_DIRS "${FRAMEWORK_INCLUDE_DIRS}" PARENT_SCOPE)
endfunction()

# =============================================================================
# Custom build step reusableAPI for <files>.ws3
# =============================================================================
function(ETKSQLite3_generate_ws3 ws3_file)
    #=======================================================
    # Get optional parameter: other dependencies using ARGN
    set(other_dependencies ${ARGN})

    #==================================================
    # Get file name of ws3_file
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

    #====================================================
    # Use Lua executable provided by ETKSQLite3 package
    if(NOT DEFINED ETKSQLite3_GENERATION_TYPE)
        message(FATAL_ERROR "ETKSQLite3_GENERATION_TYPE is not defined. Make sure you called find_package(ETKSQLite3). qt / wx / stl indicate if the generated file must be generated for Qt, STL or for wxWidgets")
    endif()

    #====================================================
    # Manage dependencies
    set(_all_dependencies
        "${ws3_file}"
        "${ETKSQLite3_SCRIPTS_DIR}/compile_ws3.lua"
        "${ETKSQLite3_SCRIPTS_DIR}/ETKSQLite3TableTemplate.h"
        "${ETKSQLite3_SCRIPTS_DIR}/ETKSQLite3TableTemplate.cpp"
    )
    if(other_dependencies)
        list(APPEND _all_dependencies ${other_dependencies})
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
                               "${ws3_file}" "${ETKSQLite3_GENERATION_TYPE}" "${out_h}" "${out_cpp}"
                       DEPENDS ${_all_dependencies}
                       COMMENT "Compiling ws3 database for ${ETKSQLite3_GENERATION_TYPE} => ${ws3_file} -> ${out_cpp}, ${out_h}"
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

# =============================================================================
# Copy ETKSQLite3 dynamic libraray into
# =============================================================================
function(ETKSQLite3_Copy_Dynamic_Libraries TARGET ETK_SQLITE3_TARGET)
    #==================================================
    # Iterate over all linked libraries
    # Only copy ETKSQLite3 DLL if it's a shared library
    get_target_property(_type ${ETK_SQLITE3_TARGET} TYPE)
    if(_type STREQUAL "SHARED_LIBRARY")
        # Add post-build copy command using generator expressions
        add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "$<TARGET_FILE:${ETK_SQLITE3_TARGET}>"
                    "$<TARGET_FILE_DIR:${TARGET}>"
            COMMENT "Copying ETKSQLite3 DLL $<TARGET_FILE:${ETK_SQLITE3_TARGET}> to target folder"
        )
    endif()
endfunction()