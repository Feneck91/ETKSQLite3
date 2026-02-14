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
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    message(STATUS "Configuring samples as standalone project")

    cmake_minimum_required(VERSION 3.16)
    project(ETKSQLite3Samples LANGUAGES CXX)

    # ============================================================
    # Build options
    # ============================================================
    option(ETK_ENABLE_QT                    "Enable Qt backend"                             ON)     # Enable STK build on Qt
    option(ETK_ENABLE_WX                    "Enable wxWidgets backend"                      ON)     # Enable STK build on wxWidgets
    option(ETK_ENABLE_STL                   "Enable STL backend"                            OFF)    # Enable STK build on STL base
endif()

if(ETK_ENABLE_QT)
    add_subdirectory(qtETKSQLite3Sample)
endif()
if(ETK_ENABLE_WX)
    add_subdirectory(wxETKSQLite3Sample)
endif()