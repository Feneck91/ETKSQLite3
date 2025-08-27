# *****************************************************************************
# Name:        CMakeLists.txt
# Library:     ETKSQLite3 for Qt
# Purpose:     CMake root file for ETKSqlite3
# Author:      Stéphane Château (Feneck91@free.fr)
# Modified by:
# Created:     2025/08/27
# Copyright:   © Stéphane Château
# Licence:     wxWindows licence
# *****************************************************************************
cmake_minimum_required(VERSION 3.29...3.31)
project(ETKSQLite3)

# Options to choose the library type to generate
# Ask to user the path where wxsqlite3 is installed
set(WXSQLITE3_DIR "Path where wxsqlite3 is installed (mandatory)" CACHE PATH "Installation path of wxsqlite3")
set(SQLITE3_DIR "Path where sqlite3 is installed (mandatory)" CACHE PATH "Installation path of sqlite3")
option(wxETKSQLite3 "Compile the wxWidgets version (wxETKSQLite3)" OFF)
option(qtETKSQLite3 "Compile the Qt version (qtETKSQLite3)" OFF)
option(boostETKSQLite3 "Compile the Boost version (boostETKSQLite3)" OFF)
option(BUILD_SHARED_LIBS "Compile to dynamic library" ON)

# ETKSQLite3 environment variables
set(CMAKE_CXX_STANDARD 11)
set(MAKING_STL_SQLITE3_LIB)

# Define configurations type to build
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel")

# New policy for CMAKE >= v3.30
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_WIN32_WINNT=0x0601)

    # Active hot reload for MSVC compilers is allowed
    if (POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo,MinSizeRel>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo,MinSizeRel>:ProgramDatabase>>")
    endif()

    # Ajoutez des options de compilation spécifiques à chaque configuration
    foreach(config ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${config} CONFIG)
        if (${CONFIG} STREQUAL "DEBUG")
            message("CONFIGURATION: DEBUG MODE DETECTED = ${CONFIG}")
            add_compile_options($<$<CONFIG:${config}>:/W4> $<$<CONFIG:${config}>:/utf-8>)
        elseif (${CONFIG} STREQUAL "RELEASE" OR ${CONFIG} STREQUAL "RELWITHDEBINFO" OR ${CONFIG} STREQUAL "MINSIZEREL")
            message("CONFIGURATION: RELEASE MODE DETECTED = ${CONFIG}")
            add_compile_options($<$<CONFIG:${config}>:/W4> $<$<CONFIG:${config}>:/utf-8>)
        else()
            message("NO MODE DETECTED, MODE = ${CONFIG}")
        endif()
    endforeach()

    string(REGEX REPLACE "/W[1-3]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # Workaround to fix warning D9025: substitution of '/W3' by '/W4'
else()
    add_compile_options(-Wall -Wextra -Wpedantic -finput-charset=Latin1 -fexec-charset=UTF-8 -Werror -fpic)
endif()

# Define common headers files
set(COMMON_HEADER_FILES
    ../include/ETKSQLite3/ETKSQLite3Column.h
    ../include/ETKSQLite3/ETKSQLite3Criterion.h
    ../include/ETKSQLite3/ETKSQLite3Database.h
    ../include/ETKSQLite3/ETKSQLite3Include.h
    ../include/ETKSQLite3/ETKSQLite3Record.h
    ../include/ETKSQLite3/ETKSQLite3Request.h
    ../include/ETKSQLite3/ETKSQLite3RequestDeleter.h
    ../include/ETKSQLite3/ETKSQLite3RequestInserter.h
    ../include/ETKSQLite3/ETKSQLite3RequestSelector.h
    ../include/ETKSQLite3/ETKSQLite3RequestUpdater.h
    ../include/ETKSQLite3/ETKSQLite3ResultSet.h
    ../include/ETKSQLite3/ETKSQLite3ResultSetBase.h
    ../include/ETKSQLite3/ETKSQLite3SharedPtr.h
    ../include/ETKSQLite3/ETKSQLite3Types.h
    ../include/ETKSQLite3/ETKSQLite3ValueBind.h
    ../include/ETKSQLite3/ETKSQLite3VariantDatas.h
    ../include/ETKSQLite3/ETKSQLite3Version.h
    ../include/ETKSQLite3/ETKSQLite3_pch.h
)

# Define common source files
set(COMMON_SOURCE_FILES
    ../src/ETKSQLite3/ETKSQLite3Column.cpp
    ../src/ETKSQLite3/ETKSQLite3Criterion.cpp
    ../src/ETKSQLite3/ETKSQLite3Database.cpp
    ../src/ETKSQLite3/ETKSQLite3Record.cpp
    ../src/ETKSQLite3/ETKSQLite3Request.cpp
    ../src/ETKSQLite3/ETKSQLite3RequestDeleter.cpp
    ../src/ETKSQLite3/ETKSQLite3RequestInserter.cpp
    ../src/ETKSQLite3/ETKSQLite3RequestSelector.cpp
    ../src/ETKSQLite3/ETKSQLite3RequestUpdater.cpp
    ../src/ETKSQLite3/ETKSQLite3ResultSetBase.cpp
    ../src/ETKSQLite3/ETKSQLite3ValueBind.cpp
    ../src/ETKSQLite3/ETKSQLite3VariantDatas.cpp
    ../src/ETKSQLite3/ETKSQLite3WxSQLite3.cpp    
)

# Define Qt specific header files (wrappers)
set(QT_HEADER_FILES
    ../include/wx4qt/wx/arrstr.h
    ../include/wx4qt/wx/buffer.h
    ../include/wx4qt/wx/confbase.h
    ../include/wx4qt/wx/datetime.h
    ../include/wx4qt/wx/dynarray.h
    ../include/wx4qt/wx/dynlib.h
    ../include/wx4qt/wx/file.h
    ../include/wx4qt/wx/filename.h
    ../include/wx4qt/wx/hashmap.h
    ../include/wx4qt/wx/longlong.h
    ../include/wx4qt/wx/regex.h
    ../include/wx4qt/wx/string.h
    ../include/wx4qt/wx/thread.h
    ../include/wx4qt/wx/tokenzr.h
    ../include/wx4qt/wx/variant.h
    ../include/wx4qt/wx/wx.h
    ../include/wx4qt/wx/wxprec.h
    ../include/wx4qt/wx/wxtype.h
)

# Define Qt specific source files
set(QT_SOURCE_FILES
    ../src/wx4qt/implements.cpp
)

set(MODULE_RC_FILE
    ../src/ETKSQLite3/resource.rc
)
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_WIN32_WINNT=0x0601)

    # Active hot reload for MSVC compilers if supported.
    if (POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo,MinSizeRel>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo,MinSizeRel>:ProgramDatabase>>")
    endif()

    # Ajoutez des options de compilation spécifiques à chaque configuration
    foreach(config ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${config} CONFIG)
        if (${CONFIG} STREQUAL "DEBUG")
            message("CONFIGURATION: DEBUG MODE DETECTED = ${CONFIG}")
            add_compile_options($<$<CONFIG:${config}>:/W4> $<$<CONFIG:${config}>:/utf-8>)
        elseif (${CONFIG} STREQUAL "RELEASE" OR ${CONFIG} STREQUAL "RELWITHDEBINFO" OR ${CONFIG} STREQUAL "MINSIZEREL")
            message("CONFIGURATION: RELEASE MODE DETECTED = ${CONFIG}")
            add_compile_options($<$<CONFIG:${config}>:/W4> $<$<CONFIG:${config}>:/utf-8> $<$<CONFIG:${config}>:/MP8>)
        else()
            message("NO MODE DETECTED, MODE = ${CONFIG}")
        endif()
    endforeach()

    string(REGEX REPLACE "/W[1-3]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # Workaround to fix warning D9025: substitution of '/W3' by '/W4'
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -finput-charset=Latin1 -fexec-charset=UTF-8 -Werror -fpic)
endif()

# Fonction to create a specific framework library
function(create_framework_library framework)
    if(${framework})
        # Define library name
        set(LIB_NAME "${framework}")

        # Définir les sources et en-têtes spécifiques
        if(framework STREQUAL "wxETKSQLite3")
            set(ETKSQLITE3_SOURCES ${COMMON_SOURCE_FILES})
            set(ETKSQLITE3_HEADERS ${COMMON_HEADER_FILES})
            find_package(wxWidgets REQUIRED)
            set(FRAMEWORK_LIBS ${wxWidgets_LIBRARIES})
            set(FRAMEWORK_DEFINITIONS USE_WXWIDGETS)
        elseif(framework STREQUAL "qtETKSQLite3")
            set(ETKSQLITE3_SOURCES ${COMMON_SOURCE_FILES} ${QT_SOURCE_FILES})
            set(ETKSQLITE3_HEADERS ${COMMON_HEADER_FILES} ${QT_HEADER_FILES})
            find_package(Qt5 REQUIRED COMPONENTS Widgets)
            set(FRAMEWORK_LIBS Qt5::Widgets)
            set(FRAMEWORK_DEFINITIONS USE_QT)
        elseif(framework STREQUAL "boostETKSQLite3")
            set(ETKSQLITE3_SOURCES ${COMMON_SOURCE_FILES})
            set(ETKSQLITE3_HEADERS ${COMMON_HEADER_FILES})
            find_package(Boost REQUIRED)
            set(FRAMEWORK_LIBS Boost::boost)
            set(FRAMEWORK_DEFINITIONS USE_BOOST)
        else()
            message(FATAL_ERROR "No target type for ETKSQLite3 is found, ${framework} is selected but not supported. Error into CMake file!")
        endif()

        source_group(
            TREE ${CMAKE_CURRENT_SOURCE_DIR}/..
            PREFIX "Source Files"
            FILES ${ETKSQLITE3_SOURCES}
        )

        source_group(
            TREE ${CMAKE_CURRENT_SOURCE_DIR}/..
            PREFIX "Header Files"
            FILES ${COMMON_HEADER_FILES}
        )

        source_group(
            TREE ${CMAKE_CURRENT_SOURCE_DIR}/..
            PREFIX "Resource Files"
            FILES ${MODULE_RC_FILE}
        )

        # Créer la librairie
        if(BUILD_SHARED_LIBS)
            set(BUILD_SHARED_LIBS_TYPE SHARED)
            message("Create shared library ${framework}")
            add_definitions(-DMAKING_ETK_SQLITE3_SHARED)
        else()
            set(BUILD_SHARED_LIBS_TYPE STATIC)
            message("Create static library ${framework}")
            add_definitions(-DMAKING_ETK_SQLITE3_LIB)
        endif()

        # Add definition
        add_definitions(-DETKSQLite3_USE_THREADS)

        # Add library
        add_library(${LIB_NAME} ${BUILD_SHARED_LIBS_TYPE} ${ETKSQLITE3_SOURCES} ${ETKSQLITE3_HEADERS} ${MODULE_RC_FILE})

        # Configure library properties
        target_include_directories(${LIB_NAME} PUBLIC ../include/ETKSQLite3 ../include/wx4qt ${WXSQLITE3_DIR}/include ${WXSQLITE3_DIR}/src ${SQLITE3_DIR})
        target_link_libraries(${LIB_NAME} PRIVATE ${FRAMEWORK_LIBS})
        target_compile_definitions(${LIB_NAME} PRIVATE ${FRAMEWORK_DEFINITIONS})

        # Define suffix depending of library type
        if(BUILD_SHARED_LIBS)
            set_target_properties(${LIB_NAME} PROPERTIES SUFFIX "-shared${CMAKE_SHARED_LIBRARY_SUFFIX}")
        else()
            set_target_properties(${LIB_NAME} PROPERTIES SUFFIX "-static${CMAKE_STATIC_LIBRARY_SUFFIX}")
        endif()

        ## Installer la librairie (optionnel)
        #install(TARGETS ${LIB_NAME}
        #    ARCHIVE DESTINATION lib
        #    LIBRARY DESTINATION lib
        #    PUBLIC_HEADER DESTINATION include/${framework}
        #)
    endif()
endfunction()

# Create library depending of options
create_framework_library(wxETKSQLite3 "wx")
create_framework_library(qtETKSQLite3 "qt")
create_framework_library(boostETKSQLite3 "boost")

# At least one option is selected
if(NOT wxETKSQLite3 AND NOT qtETKSQLite3 AND NOT boostETKSQLite3)
    message(FATAL_ERROR "No target type for ETKSQLite3 is selected. You must activate at least wxETKSQLite3, qtETKSQLite3 or boostETKSQLite3.")
endif()