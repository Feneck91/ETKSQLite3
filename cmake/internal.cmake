# *****************************************************************************
# Name:        internal.cmake
# Library:     ETKSQLite3
# Purpose:     Functions used into root CMakeLists.txt
# Author:      Stéphane Château
# Modified by:
# Created:     2026/02/10
# Copyright:   © Stéphane Château
# Licence:     wxWindows licence
# *****************************************************************************

# -----------------------------------------------------------------------------
# Helper to create ETKSQLite3 library depending on backend
# -----------------------------------------------------------------------------
function(add_framework_library FRAMEWORK_FLAG FRAMEWORK_NAME)
    if(${FRAMEWORK_FLAG})
        set(LIB_NAME ${FRAMEWORK_NAME})
        set(SOURCES ${ETKSQLITE3_SOURCES})
        set(HEADERS ${ETKSQLITE3_HEADERS})
        set(INCLUDES include/ETKSQLite3 ${WXSQLITE3_DIR}/include ${WXSQLITE3_DIR}/src)
        if(BUILD_SHARED_LIBS)
            set(RC_FILES ${ETKSQLITE3_RC_FILES})
        else()
            set(RC_FILES "")
        endif()

        if(FRAMEWORK_FLAG STREQUAL "ETK_ENABLE_WX")
            list(APPEND INCLUDES ${wxWidgets_INCLUDE_DIRS})
        elseif(FRAMEWORK_FLAG STREQUAL "ETK_ENABLE_QT")
            if(USE_QT6 AND Qt6_FOUND)
                find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
                set(QT_MAJOR_VERSION 6)
                set(QT_LIBS Qt6::Core Qt6::Widgets)
                message(STATUS "Using Qt6")
            else()
                find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
                set(QT_MAJOR_VERSION 5)
                set(QT_LIBS Qt5::Core Qt5::Widgets)
                message(STATUS "Using Qt5")
            endif()
            list(APPEND SOURCES ${QT_SOURCE_FILES})
            list(APPEND HEADERS ${QT_HEADER_FILES})
            list(APPEND INCLUDES include/wx4qt)
        elseif(FRAMEWORK_FLAG STREQUAL "ETK_ENABLE_STL")
            list(APPEND SOURCES ${STL_SOURCE_FILES})
            list(APPEND HEADERS ${STL_HEADER_FILES})
            list(APPEND INCLUDES include/wx4stl)
        else()
            message(FATAL_ERROR "No target type for ETKSQLite3 is found, ${FRAMEWORK_FLAG} is selected but not supported. Error into CMake file!")
        endif()

        # ------------------------------
        # Create source groups for IDE
        source_group(
            TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
            PREFIX "Source Files"
            FILES ${SOURCES}
        )
        source_group(
            TREE ${CMAKE_CURRENT_SOURCE_DIR}/include
            PREFIX "Header Files"
            FILES ${HEADERS}
        )
        if(BUILD_SHARED_LIBS)
            source_group(
                TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
                PREFIX "Resource Files"
                FILES ${RC_FILES}
            )
        endif()

        # ------------------------------
        # Create the library
        add_library(${LIB_NAME} ${SOURCES} ${RC_FILES} ${HEADERS})

        # ---------------------------------------
        # Include directories: build vs install
        set(BUILD_INCLUDES "")
        set(INSTALL_INCLUDES "")
        foreach(inc ${INCLUDES})
            if(IS_ABSOLUTE ${inc})
                if(inc STREQUAL "${WXSQLITE3_DIR}/include")
                    # wxsqlite3 include directory: map to install/include/wxsqlite3
                    list(APPEND BUILD_INCLUDES   "$<BUILD_INTERFACE:${inc}>")
                    list(APPEND INSTALL_INCLUDES "$<INSTALL_INTERFACE:include/wxsqlite3>")
                elseif(inc STREQUAL "${WXSQLITE3_DIR}/src")
                    # wxsqlite3 src directory → build only (never install)
                    list(APPEND BUILD_INCLUDES "$<BUILD_INTERFACE:${inc}>")
                else()
                    # Other absolute paths (wxWidgets etc.) → build only
                    list(APPEND BUILD_INCLUDES "$<BUILD_INTERFACE:${inc}>")
                endif()

            else()
                # Relative paths (include/ETKSQLite3, include/wx4qt, include/wx4stl)
                list(APPEND BUILD_INCLUDES   "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${inc}>")
                list(APPEND INSTALL_INCLUDES "$<INSTALL_INTERFACE:${inc}>")
            endif()
        endforeach()
        target_include_directories(${LIB_NAME} PUBLIC ${BUILD_INCLUDES} ${INSTALL_INCLUDES})

        # -------------------------------
        # Link Qt if needed
        if(${FRAMEWORK_FLAG} STREQUAL ETK_ENABLE_QT)
            target_link_libraries(${LIB_NAME} PRIVATE ${QT_LIBS})
        endif()

        # ---------------------------------------------
        # Compile definitions (threads, sqlite options)
        target_compile_definitions(${LIB_NAME} PRIVATE ETKSQLite3_USE_THREADS=1
                                                       $<$<BOOL:${SQLITE_ENABLE_SESSION}>:SQLITE_ENABLE_SESSION=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_MATH_FUNCTIONS}>:SQLITE_ENABLE_MATH_FUNCTIONS=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_MONOTONIC_CLOCK}>:SQLITE_ENABLE_MONOTONIC_CLOCK=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_DBSTAT_VTAB}>:SQLITE_ENABLE_DBSTAT_VTAB=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_STMTVTAB}>:SQLITE_ENABLE_STMTVTAB=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_UNLOCK_NOTIFY}>:SQLITE_ENABLE_UNLOCK_NOTIFY=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_SERIALIZE}>:SQLITE_ENABLE_SERIALIZE=1>
                                                       $<$<BOOL:${wxUSE_REGEX}>:wxUSE_REGEX=1>
                                                       $<$<BOOL:${wxUSE_THREADS}>:wxUSE_THREADS=1>
                                                       SQLITE_THREADSAFE=1
                                                       SQLITE_MAX_ATTACHED=10
                                                       SQLITE_DQS=1
                                                       SQLITE_ENABLE_EXPLAIN_COMMENTS=1
                                                       SQLITE_SOUNDEX=1
                                                       SQLITE_ENABLE_COLUMN_METADATA=1
                                                       SQLITE_SECURE_DELETE=1
                                                       SQLITE_ENABLE_DESERIALIZE=1
                                                       SQLITE_ENABLE_FTS3=1
                                                       SQLITE_ENABLE_FTS3_PARENTHESIS=1
                                                       SQLITE_ENABLE_FTS4=1
                                                       SQLITE_ENABLE_FTS5=1
                                                       SQLITE_ENABLE_JSON1=1
                                                       SQLITE_ENABLE_RTREE=1
                                                       SQLITE_ENABLE_GEOPOLY=1
                                                       SQLITE_CORE=1
                                                       SQLITE_ENABLE_EXTFUNC=1
                                                       SQLITE_ENABLE_CSV=1
                                                       SQLITE_ENABLE_VSV=1
                                                       SQLITE_ENABLE_SHA3=1
                                                       SQLITE_ENABLE_CARRAY=1
                                                       SQLITE_ENABLE_FILEIO=1
                                                       SQLITE_ENABLE_SERIES=1
                                                       SQLITE_ENABLE_UUID=1
                                                       SQLITE_ENABLE_REGEXP=1
                                                       SQLITE_TEMP_STORE=2
                                                       SQLITE_USE_URI=1
                                                       SQLITE_USER_AUTHENTICATION=1
        )

        # ----------------------------------------
        # Framework-specific compile defs + alias
        # Set ETK_SQLITE3_USE_WXWIDGETS / ETK_SQLITE3_USE_QT / ETK_SQLITE3_USE_STL
        # Used with rc file to compute dll name
        if(${FRAMEWORK_FLAG} STREQUAL ETK_ENABLE_WX)
            target_compile_definitions(${LIB_NAME} PUBLIC ETK_SQLITE3_USE_WXWIDGETS=1)
            add_library(ETKSQLite3::Wx ALIAS wxETKSQLite3)
        elseif(${FRAMEWORK_FLAG} STREQUAL ETK_ENABLE_QT)
            target_compile_definitions(${LIB_NAME} PUBLIC ETK_SQLITE3_USE_QT=1)
            add_library(ETKSQLite3::Qt ALIAS qtETKSQLite3)
        elseif(${FRAMEWORK_FLAG} STREQUAL ETK_ENABLE_STL)
            target_compile_definitions(${LIB_NAME} PUBLIC ETK_SQLITE3_USE_STL=1)
            add_library(ETKSQLite3::Stl ALIAS stlETKSQLite3)
        endif()

        # -------------------------------
        # Define library suffix
        if(BUILD_SHARED_LIBS)
            set_target_properties(${LIB_NAME} PROPERTIES SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}")
        else()
            set_target_properties(${LIB_NAME} PROPERTIES SUFFIX "_static${CMAKE_STATIC_LIBRARY_SUFFIX}")
        endif()

        # -------------------------------
        # Set MAKING_ETK_SQLITE3_* definitions
        # -------------------------------
        if(BUILD_SHARED_LIBS)
            target_compile_definitions(${LIB_NAME} PRIVATE MAKING_ETK_SQLITE3_SHARED=1)
            message(STATUS "Create shared library ${FRAMEWORK_NAME}")
        else()
            set(BUILD_SHARED_LIBS_TYPE STATIC)
            target_compile_definitions(${LIB_NAME} PRIVATE MAKING_ETK_SQLITE3_LIB=1)
            message(STATUS "Create static library ${FRAMEWORK_NAME}")
        endif()
    endif()
endfunction()

# -----------------------------------------------------------------------------
# Function for lua install
# -----------------------------------------------------------------------------
function(lua_install)
    # Install Lua executable and DLL depending of target
    if(WIN32)
        set(ETKSQLITE3_LUA_EXECUTABLE "lua52.exe")
        set(ETKSQLITE3_LUA_FOLDER "win32")
        install(FILES "${CMAKE_SOURCE_DIR}/tools/lua/win32/lua52.exe"
                      "${CMAKE_SOURCE_DIR}/tools/lua/win32/lua52.dll"
                DESTINATION bin/lua/${ETKSQLITE3_LUA_FOLDER})
    elseif(UNIX AND NOT APPLE)
        set(ETKSQLITE3_LUA_EXECUTABLE "lua52")
        set(ETKSQLITE3_LUA_FOLDER "linux")
        install(FILES "${CMAKE_SOURCE_DIR}/tools/lua/linux/lua52"
                DESTINATION bin/lua/${ETKSQLITE3_LUA_FOLDER})
    elseif(APPLE)
    endif()

    # Configure the ETKSQLite3Config.cmake for installation
    set(LUA_FOLDER ${ETKSQLITE3_LUA_FOLDER}     PARENT_SCOPE)
    set(LUA_EXE    ${ETKSQLITE3_LUA_EXECUTABLE} PARENT_SCOPE)

    # Set the type of ETKSQLite3 generation: qt/wx/stl
    if(TARGET qtETKSQLite3)
        set(LUA_GENERATION_TYPE "qt" PARENT_SCOPE)
    elseif(TARGET wxETKSQLite3)
        set(LUA_GENERATION_TYPE "wx" PARENT_SCOPE)
    elseif(TARGET stlETKSQLite3)
        set(LUA_GENERATION_TYPE "stl" PARENT_SCOPE)
    else()
        message(FATAL_ERROR "No ETKSQLite3 backend target found (qt/wx/stl)")
    endif()

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ETKSQLite3Config.cmake.in"   # fichier template
        "${CMAKE_CURRENT_BINARY_DIR}/ETKSQLite3Config.cmake"            # fichier généré
        @ONLY
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/ETKSQLite3Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/ETKSQLite3ConfigVersion.cmake"
        DESTINATION lib/cmake/ETKSQLite3
    )
endfunction()

# -----------------------------------------------------------------------------
# Function to setup Tools interface target for clients
# -----------------------------------------------------------------------------
function(tools_functions_install)
    # Target INTERFACE to expose CMake functions to customers
    add_library(ETKSQLite3Tools INTERFACE)

    # The folder that contains tools.cmake for Build and install
    target_include_directories(ETKSQLite3Tools INTERFACE $<INSTALL_INTERFACE:lib/cmake/ETKSQLite3>)

    # Alias public for the namespace ETKSQLite3::Tools
    add_library(ETKSQLite3::Tools ALIAS ETKSQLite3Tools)

    # Export target for installation
    install(TARGETS ETKSQLite3Tools EXPORT ETKSQLite3ToolsTargets)

    # Export lua script for installation
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/scripts" DESTINATION .)
endfunction()

# -----------------------------------------------------------------------------
# Function to install ETKSQLite3 library, headers, targets and config files
# This is the main "install" logic used after building ETKSQLite3
# -----------------------------------------------------------------------------
function(make_install)
    include(CMakePackageConfigHelpers)

    #=======================================
    # Configure the package config files
    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ETKSQLite3Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/ETKSQLite3Config.cmake
        INSTALL_DESTINATION lib/cmake/ETKSQLite3
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/ETKSQLite3ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    #=======================================
    # Collect library targets to install
    set(ETKSQLITE3_INSTALL_TARGETS "")
    if(TARGET wxETKSQLite3)
        list(APPEND ETKSQLITE3_INSTALL_TARGETS wxETKSQLite3)
    endif()
    if(TARGET qtETKSQLite3)
        list(APPEND ETKSQLITE3_INSTALL_TARGETS qtETKSQLite3)
        # Install Qt-specific headers (wx4qt)
        install(DIRECTORY include/wx4qt DESTINATION include FILES_MATCHING PATTERN "*.h")
    endif()
    if(TARGET stlETKSQLite3)
        list(APPEND ETKSQLITE3_INSTALL_TARGETS stlETKSQLite3)
        # Install STL-specific headers (wx4stl)
        install(DIRECTORY include/wx4stl DESTINATION include FILES_MATCHING PATTERN "*.h")
    endif()

    #====================================================
    # Install wxsqlite3 headers + SQLIte3 (amalgamation)
    install(FILES "${WXSQLITE3_DIR}/src/sqlite3mc_amalgamation.h" DESTINATION include/wxsqlite3)
    install(DIRECTORY "${WXSQLITE3_DIR}/include/wx" DESTINATION include/wxsqlite3 FILES_MATCHING PATTERN "*.h")

    #====================================================
    # Install public headers of ETKSQLite3 itself
    install(DIRECTORY include/ETKSQLite3 DESTINATION include FILES_MATCHING PATTERN "*.h")

    #====================================================
    # Compute platform-specific subfolders for lib/bin
    set(PLATFORM_DIR "")
    if(WIN32)
        set(PLATFORM_DIR "win32")
    elseif(UNIX AND NOT APPLE)
        set(PLATFORM_DIR "linux")
    elseif(APPLE)
        set(PLATFORM_DIR "macos")
    endif()

    #=======================================
    # Install the main library targets (archive, shared, runtime)
    install(TARGETS ${ETKSQLITE3_INSTALL_TARGETS}
            EXPORT ETKSQLite3Targets
            ARCHIVE DESTINATION lib/${PLATFORM_DIR}/$<CONFIG>
            LIBRARY DESTINATION lib/${PLATFORM_DIR}/$<CONFIG>
            RUNTIME DESTINATION bin/${PLATFORM_DIR}/$<CONFIG>
    )

    #=======================================
    # Install the CMake config files
    install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/ETKSQLite3Config.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/ETKSQLite3ConfigVersion.cmake
            DESTINATION lib/cmake/ETKSQLite3
    )

    #=======================================
    # Install exported targets for clients (namespace ETKSQLite3::)
    install(EXPORT ETKSQLite3Targets
            NAMESPACE ETKSQLite3::
            DESTINATION lib/cmake/ETKSQLite3
    )

    #====================================================================================
    # Export targets for the build tree (optional, for developers building ETKSQLite3)
    export(EXPORT ETKSQLite3Targets
           FILE "${CMAKE_CURRENT_BINARY_DIR}/ETKSQLite3Targets.cmake"
           NAMESPACE ETKSQLite3::
    )

    #=====================================================
    # Install the Tools interface target and its scripts
    install(FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/tools.cmake"
            DESTINATION lib/cmake/ETKSQLite3)
    install(EXPORT ETKSQLite3ToolsTargets
            NAMESPACE ETKSQLite3::
            DESTINATION lib/cmake/ETKSQLite3)

    #=======================================
    # Create a root config file for easier find_package
    # This file just includes the "real" installed ETKSQLite3Config.cmake
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/ETKSQLite3ConfigRoot.cmake"
"#
# Root config file for ETKSQLite3
#
include(\"\${CMAKE_CURRENT_LIST_DIR}/lib/cmake/ETKSQLite3/ETKSQLite3Config.cmake\")"
    )

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/ETKSQLite3ConfigRoot.cmake"
            DESTINATION .
            RENAME ETKSQLite3Config.cmake
    )
endfunction()
