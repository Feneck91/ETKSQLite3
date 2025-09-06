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

# Add Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG  v1.12.0 # v1.12.1 > In release note: This will be the last release to support C++11. Future releases will require at least C++14.
)
FetchContent_MakeAvailable(googletest)

# Add subfolder tests/ if tests is activated
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()