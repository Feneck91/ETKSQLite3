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

# Find GoogleTest package
find_package(GTest REQUIRED)
include(GoogleTest)

# Fonction to create a specific framework library
function(make_include framework)
    if(${framework})
        # Define library name
        set(LIB_NAME "${framework}")

        # =============================================================================
        # Define specific sources and headers files
        # =============================================================================
        if(framework STREQUAL "wxETKSQLite3")
            find_package(wxWidgets REQUIRED)
            #find_package(wxWidgets REQUIRED core base)
            # Link with wxWidgets libs
            #set(FRAMEWORK_LIBS wxWidgets::core wxWidgets::base)
            set(FRAMEWORK_LIBS ${wxWidgets_LIBRARIES})
            set(FRAMEWORK_DEFINITIONS ETK_SQLITE3_USE_WXWIDGETS)
            set(WX_INCLUDE_DIRS PRIVATE ${wxWidgets_INCLUDE_DIRS} PARENT_SCOPE)
            # Mandatory options for wxWidgets
            add_definitions(-DwxUSE_UNICODE=1)
            add_definitions(-DwxUSE_DATETIME=1)
            add_definitions(-DwxUSE_STL=1)
            # Inclure wxWidgets folders
            include(${wxWidgets_USE_FILE})
        elseif(framework STREQUAL "qtETKSQLite3")
            # Detect the chosen version
            if(USE_QT6)
                find_package(Qt6 REQUIRED COMPONENTS Widgets)
                set(QT_VERSION 6 CACHE INTERNAL "Qt version in use")
                message(STATUS "Using Qt6")
                set(QT_LIBRARIES Qt6::Widgets)
                set(QT_INCLUDE_DIRS ${Qt6Widgets_INCLUDE_DIRS} PARENT_SCOPE)
                # Set compile definitions
                add_compile_definitions(USE_QT6)
            else()
                find_package(Qt5 REQUIRED COMPONENTS Widgets)
                set(QT_VERSION 5 CACHE INTERNAL "Qt version in use")
                message(STATUS "Using Qt5")
                set(QT_LIBRARIES Qt5::Widgets)
                set(QT_INCLUDE_DIRS ${Qt5Widgets_INCLUDE_DIRS} PARENT_SCOPE)
                # Set compile definitions
                add_compile_definitions(USE_QT5)
            endif()
            set(FRAMEWORK_LIBS ${QT_LIBRARIES})
            set(FRAMEWORK_DEFINITIONS ETK_SQLITE3_USE_QT)
        elseif(framework STREQUAL "stlETKSQLite3")
            set(ETKSQLITE3_SOURCES ${COMMON_SOURCE_FILES} ${STL_SOURCE_FILES})
            set(ETKSQLITE3_HEADERS ${COMMON_HEADER_FILES} ${STL_HEADER_FILES})
            set(FRAMEWORK_DEFINITIONS ETK_SQLITE3_USE_STL)
        else()
            message(FATAL_ERROR "No target type for ETKSQLite3 is found, ${framework} is selected but not supported. Error into CMake file!")
        endif()

        # Configure library properties
        # =============================================================================
        # Define specific sources and headers files
        # =============================================================================
        if(framework STREQUAL "wxETKSQLite3")
            target_include_directories(${LIB_NAME} PUBLIC $(WX_INCLUDE_DIRS) ../include/ETKSQLite3 ${WXSQLITE3_DIR}/include ${WXSQLITE3_DIR}/src)
        elseif(framework STREQUAL "qtETKSQLite3")
            target_include_directories(${LIB_NAME} PUBLIC ../include/ETKSQLite3 ../include/wx4qt ${WXSQLITE3_DIR}/include ${WXSQLITE3_DIR}/src ${QT_INCLUDE_DIRS} )
        elseif(framework STREQUAL "stlETKSQLite3")
            target_include_directories(${LIB_NAME} PUBLIC ../include/ETKSQLite3 ../include/wx4stl ${WXSQLITE3_DIR}/include ${WXSQLITE3_DIR}/src)
        else()
            message(FATAL_ERROR "No target type for ETKSQLite3 is found, ${framework} is selected but not supported. Error into CMake file!")
        endif()
    endif()
endfunction()

# =============================================================================
# Create library depending of options
# =============================================================================
make_include(wxETKSQLite3 "wx")
make_include(qtETKSQLite3 "qt")
make_include(stlETKSQLite3 "stl")

# Ajouter test executable
add_executable(ETKSQLite3_Tests etksqlite3_test.cpp)

# Link test executable to the ETKSqlite3 library and GTest
target_link_libraries(ETKSQLite3_Tests PRIVATE ETKSqlite3 GTest::GTest GTest::Main)

# Add the test to CTest system
gtest_discover_tests(ETKSQLite3_Tests)