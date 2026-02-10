# *****************************************************************************
# Name:        CMakeLists.txt
# Library:     ETKSQLite3
# Purpose:     CMake root file for ETKSqlite3
# Author:      Stéphane Château
# Modified by:
# Created:     2025/08/27
# Copyright:   © Stéphane Château
# Licence:     wxWindows licence
# *****************************************************************************
cmake_minimum_required(VERSION 3.16)
project(ETKSQLite3
    VERSION 1.0.0
    LANGUAGES C CXX
)

# ============================================================
# Build options
# ============================================================
option(BUILD_SHARED_LIBS                "Build as shared library"                       ON)     # Static / Dynamic library
option(ETK_ENABLE_QT                    "Enable Qt backend"                             OFF)    # Enable STK build on Qt
option(ETK_ENABLE_WX                    "Enable wxWidgets backend"                      OFF)    # Enable STK build on wxWidgets
option(ETK_ENABLE_STL                   "Enable STL backend"                            OFF)    # Enable STK build on STL base
option(BUILD_SAMPLES                    "Build the samples"                             OFF)    # Activate samples if needed
option(BUILD_TESTS                      "Build the test suite"                          OFF)    # Activate tests if needed
if(ETK_ENABLE_QT)
    option(USE_QT6                      "Use Qt6 instead of Qt5"                        ON)     # If QT5 / QT6 are detected, allow to use Qt6 instead of Qt5
endif()

# =============================================================================
# Define installation folder
# =============================================================================

# Default CMake original value
set(_default_prefix "${CMAKE_BINARY_DIR}/install")

# Si CMake a initialisé la variable avec sa valeur par défaut
# (ou si elle est vide)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR CMAKE_INSTALL_PREFIX STREQUAL "")
    set(CMAKE_INSTALL_PREFIX "${_default_prefix}" CACHE PATH "Installation directory for ETKSQLite3" FORCE)
endif()
message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

# -----------------------------------------------------------------------------
# Auto-detect wxSQLite3 directory (sibling of ETKSQLite3)
# -----------------------------------------------------------------------------
set(WXSQLITE3_DIR "Path to wxSQLite3" CACHE PATH "Path to wxSQLite3 root")
if(NOT EXISTS "${WXSQLITE3_DIR}")
    get_filename_component(_ETK_PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
    set(_WXSQLITE3_CANDIDATE "${_ETK_PARENT_DIR}/wxsqlite3")
    if(   EXISTS "${_WXSQLITE3_CANDIDATE}/src/sqlite3mc_amalgamation.c"
       OR EXISTS "${_WXSQLITE3_CANDIDATE}/src/sqlite3mc_amalgamation.h"
       OR EXISTS "${_WXSQLITE3_CANDIDATE}/src/wxsqlite3.cpp")
        set(WXSQLITE3_DIR "${_WXSQLITE3_CANDIDATE}" CACHE PATH "Path to wxSQLite3 root directory" FORCE)

        message(STATUS "WXSQLITE3_DIR auto-detected: ${WXSQLITE3_DIR}")
    endif()
endif()

# Final validation
if(NOT DEFINED WXSQLITE3_DIR OR NOT EXISTS "${WXSQLITE3_DIR}")
    get_filename_component(_ETK_PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
    message(FATAL_ERROR
        "WXSQLITE3_DIR not set and wxSQLite3 was not found next to ETKSQLite3.\n"
        "Expected layout:\n"
        "  ${_ETK_PARENT_DIR}/\n"
        "    ├─ ETKSQLite3/\n"
        "    └─ wxsqlite3/\n"
        "Or define WXSQLITE3_DIR manually.")
endif()

# =============================================================================
# SQLite Configuration Options
# =============================================================================
# SQLite3 feature toggles (ON/OFF)
option(SQLITE_ENABLE_SESSION            "Enable Session extension"                      ON)
option(SQLITE_ENABLE_MATH_FUNCTIONS     "Enable math functions"                         OFF)
option(SQLITE_ENABLE_MONOTONIC_CLOCK    "Enable Monotonic Clock extension"              OFF)
option(SQLITE_ENABLE_DBSTAT_VTAB        "Enable dbstat virtual table"                   OFF)
option(SQLITE_ENABLE_STMTVTAB           "Enable stmt virtual tables"                    OFF)
option(SQLITE_ENABLE_UNLOCK_NOTIFY      "Enable unlock notify"                          OFF)
option(SQLITE_ENABLE_SERIALIZE          "Enable database serialization"                 OFF)
# wxSQLite3 feature toggles (ON/OFF)
option(wxUSE_REGEX                      "Use regex in wxSQLite3"                        OFF)
option(wxUSE_THREADS                    "Use threads in wxSQLite3"                      OFF)

# -----------------------------------------------------------------------------
# C++ compilation settings
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD                  11)
set(CMAKE_CXX_STANDARD_REQUIRED         ON)
set(CMAKE_POSITION_INDEPENDENT_CODE     ON)

if(MSVC)
    add_compile_options(/W4 /permissive-)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_WIN32_WINNT=0x0601)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_options(-Wno-error=overloaded-virtual)
    add_compile_options(-Wno-error=deprecated-copy)
endif()

# =============================================================================
# Qt detection
#
# Check and display options for QT5 / QT6 - Function to detect and configure Qt
# =============================================================================
if (ETK_ENABLE_QT)
    # Check if Qt6 is available
    find_package(Qt6 QUIET COMPONENTS Core)

    # Check if Qt5 is available
    find_package(Qt5 QUIET COMPONENTS Widgets)

    # If both versions are available, let the user choose
    if(Qt6_FOUND AND Qt5_FOUND)
        if(NOT DEFINED USE_QT6)
            # Option to choose between Qt5 and Qt6
            option(USE_QT6 "Use Qt6 instead of Qt5" OFF)
        endif()
    else()
        unset(USE_QT6 CACHE)  # Remove option from CMake cache
    endif()
endif()

# =============================================================================
# Define common headers files
# =============================================================================
set(ETKSQLITE3_HEADERS
    include/ETKSQLite3/ETKSQLite3Column.h
    include/ETKSQLite3/ETKSQLite3Criterion.h
    include/ETKSQLite3/ETKSQLite3Database.h
    include/ETKSQLite3/ETKSQLite3ImportExport.h
    include/ETKSQLite3/ETKSQLite3Include.h
    include/ETKSQLite3/ETKSQLite3Record.h
    include/ETKSQLite3/ETKSQLite3Request.h
    include/ETKSQLite3/ETKSQLite3RequestDeleter.h
    include/ETKSQLite3/ETKSQLite3RequestInserter.h
    include/ETKSQLite3/ETKSQLite3RequestSelector.h
    include/ETKSQLite3/ETKSQLite3RequestUpdater.h
    include/ETKSQLite3/ETKSQLite3ResultSet.h
    include/ETKSQLite3/ETKSQLite3ResultSetBase.h
    include/ETKSQLite3/ETKSQLite3SharedPtr.h
    include/ETKSQLite3/ETKSQLite3Types.h
    include/ETKSQLite3/ETKSQLite3ValueBind.h
    include/ETKSQLite3/ETKSQLite3VariantDatas.h
    include/ETKSQLite3/ETKSQLite3Version.h
    include/ETKSQLite3/ETKSQLite3_pch.h
)

# =============================================================================
# Define common source files
# =============================================================================
set(ETKSQLITE3_SOURCES
    src/ETKSQLite3/ETKSQLite3Column.cpp
    src/ETKSQLite3/ETKSQLite3Criterion.cpp
    src/ETKSQLite3/ETKSQLite3Database.cpp
    src/ETKSQLite3/ETKSQLite3Record.cpp
    src/ETKSQLite3/ETKSQLite3Request.cpp
    src/ETKSQLite3/ETKSQLite3RequestDeleter.cpp
    src/ETKSQLite3/ETKSQLite3RequestInserter.cpp
    src/ETKSQLite3/ETKSQLite3RequestSelector.cpp
    src/ETKSQLite3/ETKSQLite3RequestUpdater.cpp
    src/ETKSQLite3/ETKSQLite3ResultSetBase.cpp
    src/ETKSQLite3/ETKSQLite3ValueBind.cpp
    src/ETKSQLite3/ETKSQLite3VariantDatas.cpp
    src/ETKSQLite3/ETKSQLite3WxSQLite3.cpp
    src/ETKSQLite3/ETKSQLite3WxSQLite3.c
)

# =============================================================================
# Define resource file
# =============================================================================
set(ETKSQLITE3_RC_FILES
    src/ETKSQLite3/resource.rc
)

# =============================================================================
# Define Qt specific header files (wrappers)
# =============================================================================
set(QT_HEADER_FILES
    include/wx4qt/wx/arrstr.h
    include/wx4qt/wx/buffer.h
    include/wx4qt/wx/confbase.h
    include/wx4qt/wx/datetime.h
    include/wx4qt/wx/dynarray.h
    include/wx4qt/wx/file.h
    include/wx4qt/wx/filename.h
    include/wx4qt/wx/messagedialog.h
    include/wx4qt/wx/hashmap.h
    include/wx4qt/wx/longlong.h
    include/wx4qt/wx/logger.h
    include/wx4qt/wx/regex.h
    include/wx4qt/wx/string.h
    include/wx4qt/wx/thread.h
    include/wx4qt/wx/tokenzr.h
    include/wx4qt/wx/variant.h
    include/wx4qt/wx/wx.h
    include/wx4qt/wx/wxprec.h
    include/wx4qt/wx/wxtype.h
)

# =============================================================================
# Define Qt specific source files (wrappers implementation)
# =============================================================================
set(QT_SOURCE_FILES
    # Internal files for library
    src/wx4qt/implements.cpp
    src/wx4qt/file.cpp
    src/wx4qt/filename.cpp
    src/wx4qt/messagedialog.cpp
    src/wx4qt/regex.cpp
    src/wx4qt/thread.cpp
    src/wx4qt/tokenzr.cpp
    # File that should be exported
    src/wx4qt/buffer.cpp
    src/wx4qt/confbase.cpp
    src/wx4qt/datetime.cpp
    src/wx4qt/dynarray.cpp
    src/wx4qt/logger.cpp
    src/wx4qt/longlong.cpp
    src/wx4qt/string.cpp
    src/wx4qt/variant.cpp
)

# =============================================================================
# Define STL specific source files (wrappers implementation)
# =============================================================================
set(STL_SOURCE_FILES
    # Internal files for library
    src/wx4stl/implements.cpp
    src/wx4stl/file.cpp
    src/wx4stl/filename.cpp
    src/wx4stl/messagedialog.cpp
    src/wx4stl/regex.cpp
    src/wx4stl/thread.cpp
    src/wx4stl/tokenzr.cpp
    # File that should be exported
    src/wx4stl/buffer.cpp
    src/wx4stl/confbase.cpp
    src/wx4stl/datetime.cpp
    src/wx4stl/dynarray.cpp
    src/wx4stl/logger.cpp
    src/wx4stl/longlong.cpp
    src/wx4stl/string.cpp
    src/wx4stl/variant.cpp
)

# =============================================================================
# Define STL specific header files (wrappers)
# =============================================================================
set(STL_HEADER_FILES
    include/wx4stl/wx/arrstr.h
    include/wx4stl/wx/buffer.h
    include/wx4stl/wx/confbase.h
    include/wx4stl/wx/datetime.h
    include/wx4stl/wx/dynarray.h
    include/wx4stl/wx/file.h
    include/wx4stl/wx/filename.h
    include/wx4stl/wx/messagedialog.h
    include/wx4stl/wx/hashmap.h
    include/wx4stl/wx/longlong.h
    include/wx4stl/wx/logger.h
    include/wx4stl/wx/regex.h
    include/wx4stl/wx/string.h
    include/wx4stl/wx/thread.h
    include/wx4stl/wx/tokenzr.h
    include/wx4stl/wx/variant.h
    include/wx4stl/wx/wx.h
    include/wx4stl/wx/wxprec.h
    include/wx4stl/wx/wxtype.h
)

# -----------------------------------------------------------------------------
# Helper to choose the backend
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

        if(${FRAMEWORK_FLAG} STREQUAL ETK_ENABLE_WX)
            list(APPEND INCLUDES ${wxWidgets_INCLUDE_DIRS})
        elseif(${FRAMEWORK_FLAG} STREQUAL ETK_ENABLE_QT)
            if(USE_QT6 AND Qt6_FOUND)
                find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
                set(QT_MAJOR_VERSION 6)
                set(QT_LIBS Qt6::Core Qt6::Widgets)
                message(STATUS "Using Qt6")
            else()
                find_package(Qt5 COMPONENTS Core REQUIRED)
                set(QT_MAJOR_VERSION 5)
                set(QT_LIBS Qt5::Core Qt5::Widgets)
                message(STATUS "Using Qt5")
            endif()
            list(APPEND SOURCES ${QT_SOURCE_FILES})
            list(APPEND HEADERS ${QT_HEADER_FILES})
            list(APPEND INCLUDES include/wx4qt)
        elseif(${FRAMEWORK_FLAG} STREQUAL ETK_ENABLE_STL)
            list(APPEND SOURCES ${STL_SOURCE_FILES})
            list(APPEND HEADERS ${STL_HEADER_FILES})
            list(APPEND INCLUDES include/wx4stl)
        else()
            message(FATAL_ERROR "No target type for ETKSQLite3 is found, ${FRAMEWORK_FLAG} is selected but not supported. Error into CMake file!")
        endif()

        # Create Hierarchy
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

        # Create library
        add_library(${LIB_NAME} ${SOURCES} ${RC_FILE} ${HEADERS})

        # Include directories
        target_include_directories(${LIB_NAME} PUBLIC ${INCLUDES})

        # Link Qt if needed
        if(${FRAMEWORK_FLAG} STREQUAL ETK_ENABLE_QT)
            target_link_libraries(${LIB_NAME} PRIVATE ${QT_LIBS})
        endif()

        # Define multithreading
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

        # Define suffix depending of library type
        if(BUILD_SHARED_LIBS)
            set_target_properties(${LIB_NAME} PROPERTIES SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}")
        else()
            set_target_properties(${LIB_NAME} PROPERTIES SUFFIX "_static${CMAKE_STATIC_LIBRARY_SUFFIX}")
        endif()
        
        # =============================================================================
        # Copy result to bin / lib
        # =============================================================================
        if(BUILD_SHARED_LIBS)
            # Copy dll / so files into bin/ directory
            if (WIN32)
                # Define ouput folder for windows
                set(DESTINATION_DIR_BIN "${CMAKE_CURRENT_SOURCE_DIR}/build/bin/win32")
            else()
                # Define ouput folder for linux
                set(DESTINATION_DIR_BIN "${CMAKE_CURRENT_SOURCE_DIR}/build/bin/linux")
            endif()
            # Create destination folder if it doesn't exits
            file(MAKE_DIRECTORY "${DESTINATION_DIR_BIN}")
            add_custom_command(TARGET ${LIB_NAME} POST_BUILD
                               COMMAND ${CMAKE_COMMAND} -E make_directory
                               "${DESTINATION_DIR_BIN}/$<CONFIG>/")
            add_custom_command(TARGET ${LIB_NAME} POST_BUILD
                               COMMAND ${CMAKE_COMMAND} -E copy
                               "$<TARGET_FILE:${LIB_NAME}>"
                               "${DESTINATION_DIR_BIN}/$<CONFIG>/")
            set(BUILD_SHARED_LIBS_TYPE SHARED)
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
# Build the selected framework
# -----------------------------------------------------------------------------
add_framework_library(ETK_ENABLE_WX wxETKSQLite3)
add_framework_library(ETK_ENABLE_QT qtETKSQLite3)
add_framework_library(ETK_ENABLE_STL stlETKSQLite3)

# -----------------------------------------------------------------------------
# Check if at least one target is enabled
# -----------------------------------------------------------------------------
if(NOT ETK_ENABLE_WX AND NOT ETK_ENABLE_QT AND NOT ETK_ENABLE_STL)
    message(FATAL_ERROR "You must enable at least one of ETK_ENABLE_WX, ETK_ENABLE_QT, ETK_ENABLE_STL!")
endif()

include(CMakePackageConfigHelpers)

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

set(ETKSQLITE3_INSTALL_TARGETS "")
if(TARGET wxETKSQLite3)
    list(APPEND ETKSQLITE3_INSTALL_TARGETS wxETKSQLite3)
endif()

if(TARGET qtETKSQLite3)
    list(APPEND ETKSQLITE3_INSTALL_TARGETS qtETKSQLite3)
endif()

if(TARGET stlETKSQLite3)
    list(APPEND ETKSQLITE3_INSTALL_TARGETS stlETKSQLite3)
endif()

install(
    DIRECTORY include/ETKSQLite3
    DESTINATION include
)

install(
    TARGETS ${ETKSQLITE3_INSTALL_TARGETS}
    EXPORT ETKSQLite3Targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ETKSQLite3Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/ETKSQLite3ConfigVersion.cmake
    DESTINATION lib/cmake/ETKSQLite3
)

if(BUILD_SAMPLES)
    add_subdirectory(samples)
endif()

message(STATUS "ETKSQLite3 configured successfully")
