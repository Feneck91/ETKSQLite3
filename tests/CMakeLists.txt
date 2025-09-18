# *****************************************************************************
# Name:        CMakeLists.txt
# Library:     ETKSQLite3
# Purpose:     CMake for tests for ETKSqlite3
# Author:      Stéphane Château (Feneck91@free.fr)
# Modified by:
# Created:     2025/09/06
# Copyright:   © Stéphane Château
# Licence:     wxWindows licence
# *****************************************************************************
cmake_minimum_required(VERSION 3.29...3.31)
project(ETKSQLite3_Tests)

# =============================================================================
# Options to choose the library type to generate
# =============================================================================
option(wxETKSQLite3                     "Compile the wxWidgets version (wxETKSQLite3)"  OFF)
option(qtETKSQLite3                     "Compile the Qt version (qtETKSQLite3)"         OFF)
option(stlETKSQLite3                    "Compile the STL version (stlETKSQLite3)"       OFF)

# =============================================================================
# Check and display options for QT5 / QT6 - Function to detect and configure Qt
# =============================================================================
if (qtETKSQLite3)
    # Check if Qt6 is available
    find_package(Qt6 QUIET COMPONENTS Widgets)

    # Check if Qt5 is available
    find_package(Qt5 QUIET COMPONENTS Widgets)

    # If both versions are available, let the user choose
    if(Qt6_FOUND AND Qt5_FOUND)
        message(STATUS "Qt5 and Qt6 are installed. Using Qt${QT_VERSION} (change with -DUSE_QT6=ON/OFF).")
        if(NOT DEFINED USE_QT6)
            # Option to choose between Qt5 and Qt6
            option(USE_QT6 "Use Qt6 instead of Qt5" OFF)
        endif()
    else()
        unset(USE_QT6 CACHE)  # Remove option from CMake cache
        if(Qt6_FOUND)
            #set(USE_QT6 ON CACHE BOOL "Use Qt6 instead of Qt5" FORCE)
            message(STATUS "Only Qt6 is installed. Using Qt6.")
        elseif(Qt5_FOUND)
            #set(USE_QT6 OFF CACHE BOOL "Use Qt6 instead of Qt5" FORCE)
            message(STATUS "Only Qt5 is installed. Using Qt5.")
        else()
            message(FATAL_ERROR "Neither Qt5 nor Qt6 found.")
        endif()
    endif()
endif()

# =============================================================================
# Find GoogleTest package
# =============================================================================
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG  v1.12.0 # v1.12.1 > In release note: This will be the last release to support C++11. Future releases will require at least C++14.
)

# Remove inheritance système
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# =============================================================================
# Build
# =============================================================================
add_definitions(-DUSING_ETK_SQLITE3_LIB)

# Ajouter test executable
add_executable(ETKSQLite3_Tests etksqlite3_test.cpp)

link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../build/lib/${FOLDER_OS}/${CONFIGURATION}")

# Link to qtETKSQLite3 and Qt5
target_link_libraries(ETKSQLite3_Tests PRIVATE qtETKSQLite3_static.lib Qt5::Widgets)

# Include directories
target_include_directories(ETKSQLite3_Tests PRIVATE "../../include/ETKSQLite3" "../../include/wx4qt" "${WXSQLITE3_DIR}/include" "${WXSQLITE3_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}")

# Compile definitions
target_compile_definitions(ETKSQLite3_Tests PRIVATE ETK_SQLITE3_USE_QT)

# Link test executable to the ETKSqlite3 library and GTest
#target_link_libraries(ETKSQLite3_Tests PRIVATE ETK::ETKSqlite3 GTest::GTest GTest::Main)

target_link_libraries(ETKSQLite3_Tests PRIVATE GTest::gtest GTest::gtest_main)
# Add the test to CTest system
gtest_discover_tests(ETKSQLite3_Tests)