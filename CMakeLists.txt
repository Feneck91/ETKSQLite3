# *****************************************************************************
# Name:        CMakeLists.txt
# Library:     ETKSQLite3
# Purpose:     CMake root file for ETKSqlite3
# Author:      Stéphane Château (Feneck91@free.fr)
# Modified by:
# Created:     2025/08/27
# Copyright:   © Stéphane Château
# Licence:     wxWindows licence
# *****************************************************************************
cmake_minimum_required(VERSION 3.29...3.31)
project(ETKSQLite3_Suite)

# Define standard C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Activer les tests si nécessaire
option(BUILD_TESTS "Build the test suite" OFF)

# Add le sub folder build to build the ETKSQLite3 lib
add_subdirectory(build)

# Add subfolder tests/ if tests is activated
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()