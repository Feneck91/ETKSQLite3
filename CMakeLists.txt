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
# Load functions
# ============================================================
include(cmake/internal.cmake)

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

# If CMake or user have initialized the variable CMAKE_INSTALL_PREFIX don't change it
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR NOT DEFINED CMAKE_INSTALL_PREFIX OR CMAKE_INSTALL_PREFIX STREQUAL "")
    set(CMAKE_INSTALL_PREFIX "${_default_prefix}" CACHE PATH "Installation directory for ETKSQLite3" FORCE)
    message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
endif()

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
        "Expected layout for automatic detection:\n"
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

# =============================================================================
# C++ compilation settings
# =============================================================================
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
# Lua compilation, this tools is needed to compile ws3 files
#                  (Release compilation only)
# =============================================================================
add_subdirectory(tools/lua)

# =============================================================================
# Qt detection
#
# Check and display options for QT5 / QT6 - Function to detect and configure Qt
# =============================================================================
if (ETK_ENABLE_QT)
    # Check if Qt6 is available
    find_package(Qt6 QUIET COMPONENTS Core)

    # Check if Qt5 is available
    find_package(Qt5 QUIET COMPONENTS Core)

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

# -----------------------------------------------------------------------------
# Make installation
# -----------------------------------------------------------------------------
lua_install()
tools_functions_install()
make_install()

if(BUILD_SAMPLES)
    add_subdirectory(samples)
endif()

message(STATUS "ETKSQLite3 configured successfully")
