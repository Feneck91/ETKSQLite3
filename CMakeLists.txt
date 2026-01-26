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
option(BUILD_SHARED_LIBS                "Build as shared library"                       ON)
option(ETK_ENABLE_QT                    "Enable Qt backend"                             OFF)
option(ETK_ENABLE_WX                    "Enable wxWidgets backend"                      OFF)
option(ETK_ENABLE_STL                   "Enable STL backend"                            OFF)
option(USE_QT6                          "Use Qt6 instead of Qt5"                        ON)

# =============================================================================
# SQLite Configuration Options
# =============================================================================
# Feature toggles (ON/OFF)
option(SQLITE_ENABLE_COLUMN_METADATA    "Enable column metadata support"                ON)
option(SQLITE_ENABLE_JSON1              "Enable JSON1 extension"                        OFF)
option(SQLITE_ENABLE_FTS3               "Enable FTS3 (Full-Text Search) extension"      ON)
option(SQLITE_ENABLE_FTS4               "Enable FTS4 extension"                         OFF)
option(SQLITE_ENABLE_FTS5               "Enable FTS5 extension"                         OFF)
option(SQLITE_ENABLE_RTREE              "Enable RTREE (R*-tree) extension"              OFF)
option(SQLITE_ENABLE_GEOPOLY            "Enable GEOPOLY (geopolymer) extension"         OFF)
option(SQLITE_ENABLE_SESSION            "Enable Session extension"                      ON)
option(SQLITE_ENABLE_MATH_FUNCTIONS     "Enable math functions"                         OFF)
option(SQLITE_ENABLE_MONOTONIC_CLOCK    "Enable Monotonic Clock extension"              OFF)
option(SQLITE_ENABLE_DBSTAT_VTAB        "Enable dbstat virtual table"                   OFF)
option(SQLITE_ENABLE_EXPLAIN_COMMENTS   "Enable EXPLAIN comments"                       OFF)
option(SQLITE_ENABLE_STMTVTAB           "Enable stmt virtual tables"                    OFF)
option(SQLITE_ENABLE_UNLOCK_NOTIFY      "Enable unlock notify"                          OFF)
option(SQLITE_ENABLE_SERIALIZE          "Enable database serialization"                 OFF)
option(SQLITE_ENABLE_DESERIALIZE        "Enable database deserialization"               OFF)
option(wxUSE_REGEX                      "Use regex in wxSQLite3"                        OFF)
option(wxUSE_THREADS                    "Use threads in wxSQLite3"                      OFF)
option(SQLITE_THREADSAFE                ""                                              OFF)
option(SQLITE_MAX_ATTACHED              ""                                              OFF)
option(SQLITE_DQS                       ""                                              OFF)
option(SQLITE_SOUNDEX                   ""                                              OFF)
option(SQLITE_SECURE_DELETE             ""                                              OFF)
option(SQLITE_ENABLE_FTS3_PARENTHESIS   ""                                              OFF)
option(SQLITE_CORE                      ""                                              OFF)
option(SQLITE_ENABLE_EXTFUNC            ""                                              OFF)
option(SQLITE_ENABLE_CSV                ""                                              OFF)
option(SQLITE_ENABLE_VSV                ""                                              OFF)
option(SQLITE_ENABLE_SHA3               ""                                              OFF)
option(SQLITE_ENABLE_CARRAY             ""                                              OFF)
option(SQLITE_ENABLE_FILEIO             ""                                              OFF)
option(SQLITE_ENABLE_SERIES             ""                                              OFF)
option(SQLITE_ENABLE_UUID               ""                                              OFF)
option(SQLITE_ENABLE_REGEXP             ""                                              OFF)
option(SQLITE_TEMP_STORE                ""                                              OFF)
option(SQLITE_USE_URI                   ""                                              OFF)
option(SQLITE_USER_AUTHENTICATION       ""                                              OFF)

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

# -----------------------------------------------------------------------------
# wxSQLite3 path
# -----------------------------------------------------------------------------
set(WXSQLITE3_DIR "Path to wxSQLite3" CACHE PATH "Path to wxSQLite3 root")

if(NOT EXISTS "${WXSQLITE3_DIR}")
    message(FATAL_ERROR "wxSQLite3 path does not exist: ${WXSQLITE3_DIR}")
endif()

# =============================================================================
# Define installation folder
# =============================================================================
if (MSVC)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "Installation directory for ETKSQLite3 (Windows)")
else()
    # Linux/macOS, keep default
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "Installation directory for ETKSQLite3 (Linux/macOS)")
endif()

# ============================================================
# Qt detection
# ============================================================
if(ETK_ENABLE_QT)
    if(USE_QT6)
        find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
        set(QT_TARGET Qt6::Widgets)
        set(QT_MAJOR_VERSION 6)
        message(STATUS "Using Qt6")
    else()
        find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
        set(QT_TARGET Qt5::Widgets)
        set(QT_MAJOR_VERSION 5)
        message(STATUS "Using Qt5")
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

        if(${FRAMEWORK_FLAG} STREQUAL ETK_ENABLE_WX)
            list(APPEND INCLUDES ${wxWidgets_INCLUDE_DIRS})
        elseif(${FRAMEWORK_FLAG} STREQUAL ETK_ENABLE_QT)
            # Detect Qt version
            if(USE_QT6)
                find_package(Qt6 COMPONENTS Core QUIET)
            endif()
            if(USE_QT6 AND Qt6_FOUND)
                find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
                set(QT_MAJOR_VERSION 6)
                set(QT_LIBS Qt6::Core Qt6::Widgets)
            else()
                find_package(Qt5 COMPONENTS Core REQUIRED)
                set(QT_MAJOR_VERSION 5)
                set(QT_LIBS Qt5::Core Qt5::Widgets)
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
        # Create library
        add_library(${LIB_NAME} ${SOURCES} ${HEADERS})

        # Include directories
        target_include_directories(${LIB_NAME} PUBLIC ${INCLUDES})

        # Link Qt if needed
        if(${FRAMEWORK_FLAG} STREQUAL ETK_ENABLE_QT)
            target_link_libraries(${LIB_NAME} PRIVATE ${QT_LIBS})
        endif()

        # Define multithreading
        target_compile_definitions(${LIB_NAME} PRIVATE ETKSQLite3_USE_THREADS=1
                                                       $<$<BOOL:${SQLITE_ENABLE_COLUMN_METADATA}>:SQLITE_ENABLE_COLUMN_METADATA=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_JSON1}>:SQLITE_ENABLE_JSON1=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_FTS3}>:SQLITE_ENABLE_FTS3=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_FTS4}>:SQLITE_ENABLE_FTS4=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_FTS5}>:SQLITE_ENABLE_FTS5=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_RTREE}>:SQLITE_ENABLE_RTREE=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_GEOPOLY}>:SQLITE_ENABLE_GEOPOLY=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_SESSION}>:SQLITE_ENABLE_SESSION=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_MATH_FUNCTIONS}>:SQLITE_ENABLE_MATH_FUNCTIONS=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_MONOTONIC_CLOCK}>:SQLITE_ENABLE_MONOTONIC_CLOCK=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_DBSTAT_VTAB}>:SQLITE_ENABLE_DBSTAT_VTAB=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_EXPLAIN_COMMENTS}>:SQLITE_ENABLE_EXPLAIN_COMMENTS=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_STMTVTAB}>:SQLITE_ENABLE_STMTVTAB=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_UNLOCK_NOTIFY}>:SQLITE_ENABLE_UNLOCK_NOTIFY=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_SERIALIZE}>:SQLITE_ENABLE_SERIALIZE=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_DESERIALIZE}>:SQLITE_ENABLE_DESERIALIZE=1>
                                                       $<$<BOOL:${wxUSE_REGEX}>:wxUSE_REGEX=1>
                                                       $<$<BOOL:${wxUSE_THREADS}>:wxUSE_THREADS=1>
                                                       $<$<BOOL:${SQLITE_THREADSAFE}>:SQLITE_THREADSAFE=1>
                                                       $<$<BOOL:${SQLITE_MAX_ATTACHED}>:SQLITE_MAX_ATTACHED=10>
                                                       $<$<BOOL:${SQLITE_DQS}>:SQLITE_DQS=1>
                                                       $<$<BOOL:${SQLITE_SOUNDEX}>:SQLITE_SOUNDEX=1>
                                                       $<$<BOOL:${SQLITE_SECURE_DELETE}>:SQLITE_SECURE_DELETE=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_FTS3_PARENTHESIS}>:SQLITE_ENABLE_FTS3_PARENTHESIS=1>
                                                       $<$<BOOL:${SQLITE_CORE}>:SQLITE_CORE=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_EXTFUNC}>:SQLITE_ENABLE_EXTFUNC=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_CSV}>:SQLITE_ENABLE_CSV=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_VSV}>:SQLITE_ENABLE_VSV=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_SHA3}>:SQLITE_ENABLE_SHA3=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_CARRAY}>:SQLITE_ENABLE_CARRAY=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_FILEIO}>:SQLITE_ENABLE_FILEIO=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_SERIES}>:SQLITE_ENABLE_SERIES=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_UUID}>:SQLITE_ENABLE_UUID=1>
                                                       $<$<BOOL:${SQLITE_ENABLE_REGEXP}>:SQLITE_ENABLE_REGEXP=1>
                                                       $<$<BOOL:${SQLITE_TEMP_STORE}>:SQLITE_TEMP_STORE=2>
                                                       $<$<BOOL:${SQLITE_USE_URI}>:SQLITE_USE_URI=1>
                                                       $<$<BOOL:${SQLITE_USER_AUTHENTICATION}>:SQLITE_USER_AUTHENTICATION=1>
        )
        if(FRAMEWORK_FLAG STREQUAL ETK_ENABLE_WX)
            target_compile_definitions(${LIB_NAME} PUBLIC ETK_SQLITE3_USE_WXWIDGETS)
        elseif(FRAMEWORK_FLAG STREQUAL ETK_ENABLE_QT)
            target_compile_definitions(${LIB_NAME} PUBLIC ETK_SQLITE3_USE_QT)
        elseif(FRAMEWORK_FLAG STREQUAL ETK_ENABLE_STL)
            target_compile_definitions(${LIB_NAME} PUBLIC ETK_SQLITE3_USE_STL)
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
            message("Create shared library ${framework}")
            add_definitions(-DMAKING_ETK_SQLITE3_SHARED)
        else()
            set(BUILD_SHARED_LIBS_TYPE STATIC)
            message("Create static library ${framework}")
            add_definitions(-DMAKING_ETK_SQLITE3_LIB)
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

message(STATUS "ETKSQLite3 configured successfully")
