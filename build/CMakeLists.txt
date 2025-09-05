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
project(ETKSQLite3)

# =============================================================================
# Ask to user the path where wxsqlite3 is installed
# =============================================================================
set(WXSQLITE3_DIR           "Path where wxsqlite3 is installed (mandatory)" CACHE PATH      "Installation path of wxsqlite3")

# =============================================================================
# Define installation folder
# =============================================================================
if (MSVC)
    # Windows, install into build folder by default
    #unset(CMAKE_INSTALL_PREFIX CACHE)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "Installation directory for ETKSQLite3 (Windows)")
else()
    # Linux/macOS, keep default
    set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE PATH "Installation directory for ETKSQLite3 (Linux/macOS)")
endif()

# =============================================================================
# Dynamic or static library
# =============================================================================
option(BUILD_SHARED_LIBS                "Compile to dynamic library"                    ON)

# =============================================================================
# Options to choose the library type to generate
# =============================================================================
option(wxETKSQLite3                     "Compile the wxWidgets version (wxETKSQLite3)"  OFF)
option(qtETKSQLite3                     "Compile the Qt version (qtETKSQLite3)"         OFF)
option(stlETKSQLite3                    "Compile the STL version (stlETKSQLite3)"       OFF)

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
# ETKSQLite3 environment variables
# =============================================================================
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
    add_definitions(-DSQLITE_THREADSAFE=1)
    add_definitions(-DSQLITE_MAX_ATTACHED=10)
    add_definitions(-DSQLITE_DQS=0)
    add_definitions(-DSQLITE_ENABLE_EXPLAIN_COMMENTS=1)
    add_definitions(-DSQLITE_SOUNDEX=1)
    add_definitions(-DSQLITE_ENABLE_COLUMN_METADATA=1)
    add_definitions(-DSQLITE_SECURE_DELETE=1)
    add_definitions(-DSQLITE_ENABLE_DESERIALIZE=1)
    add_definitions(-DSQLITE_ENABLE_FTS3=1)
    add_definitions(-DSQLITE_ENABLE_FTS3_PARENTHESIS=1)
    add_definitions(-DSQLITE_ENABLE_FTS4=1)
    add_definitions(-DSQLITE_ENABLE_FTS5=1)
    add_definitions(-DSQLITE_ENABLE_JSON1=1)
    add_definitions(-DSQLITE_ENABLE_RTREE=1)
    add_definitions(-DSQLITE_ENABLE_GEOPOLY=1)
    add_definitions(-DSQLITE_CORE=1)
    add_definitions(-DSQLITE_ENABLE_EXTFUNC=1)
    add_definitions(-DSQLITE_ENABLE_CSV=1)
    add_definitions(-DSQLITE_ENABLE_VSV=1)
    add_definitions(-DSQLITE_ENABLE_SHA3=1)
    add_definitions(-DSQLITE_ENABLE_CARRAY=1)
    add_definitions(-DSQLITE_ENABLE_FILEIO=1)
    add_definitions(-DSQLITE_ENABLE_SERIES=1)
    add_definitions(-DSQLITE_ENABLE_UUID=1)
    add_definitions(-DSQLITE_ENABLE_REGEXP=1)
    add_definitions(-DSQLITE_TEMP_STORE=2)
    add_definitions(-DSQLITE_USE_URI=1)
    add_definitions(-DSQLITE_USER_AUTHENTICATION=1)
    add_definitions(-DwxUSE_REGEX=1)
    add_definitions(-DwxUSE_THREADS=1)

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

# =============================================================================
# Define common headers files
# =============================================================================
set(COMMON_HEADER_FILES
    ../include/ETKSQLite3/ETKSQLite3Column.h
    ../include/ETKSQLite3/ETKSQLite3Criterion.h
    ../include/ETKSQLite3/ETKSQLite3Database.h
    ../include/ETKSQLite3/ETKSQLite3ImportExport.h
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

# =============================================================================
# Define common source files
# =============================================================================
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
    ../src/ETKSQLite3/ETKSQLite3WxSQLite3.c
)

# =============================================================================
# Define Qt specific header files (wrappers)
# =============================================================================
set(QT_HEADER_FILES
    ../include/wx4qt/wx/arrstr.h
    ../include/wx4qt/wx/buffer.h
    ../include/wx4qt/wx/confbase.h
    ../include/wx4qt/wx/datetime.h
    ../include/wx4qt/wx/dynarray.h
    ../include/wx4qt/wx/dynlib.h
    ../include/wx4qt/wx/file.h
    ../include/wx4qt/wx/filename.h
    ../include/wx4qt/wx/messagedialog.h
    ../include/wx4qt/wx/hashmap.h
    ../include/wx4qt/wx/longlong.h
    ../include/wx4qt/wx/logger.h
    ../include/wx4qt/wx/regex.h
    ../include/wx4qt/wx/string.h
    ../include/wx4qt/wx/thread.h
    ../include/wx4qt/wx/tokenzr.h
    ../include/wx4qt/wx/variant.h
    ../include/wx4qt/wx/wx.h
    ../include/wx4qt/wx/wxprec.h
    ../include/wx4qt/wx/wxtype.h
)

# =============================================================================
# Define Qt specific source files (wrappers implementation)
# =============================================================================
set(QT_SOURCE_FILES
    # Internal files for library
    ../src/wx4qt/implements.cpp
    ../src/wx4qt/dynlib.cpp
    ../src/wx4qt/file.cpp
    ../src/wx4qt/filename.cpp
    ../src/wx4qt/messagedialog.cpp
    ../src/wx4qt/regex.cpp
    ../src/wx4qt/thread.cpp
    ../src/wx4qt/tokenzr.cpp
    # File that should be exported
    ../src/wx4qt/buffer.cpp
    ../src/wx4qt/confbase.cpp
    ../src/wx4qt/datetime.cpp
    ../src/wx4qt/dynarray.cpp
    ../src/wx4qt/logger.cpp
    ../src/wx4qt/longlong.cpp
    ../src/wx4qt/string.cpp
    ../src/wx4qt/variant.cpp
)

# =============================================================================
# Define Qt specific header files (wrappers)
# =============================================================================
set(QT_HEADER_FILES
    ../include/wx4qt/wx/arrstr.h
    ../include/wx4qt/wx/buffer.h
    ../include/wx4qt/wx/confbase.h
    ../include/wx4qt/wx/datetime.h
    ../include/wx4qt/wx/dynarray.h
    ../include/wx4qt/wx/dynlib.h
    ../include/wx4qt/wx/file.h
    ../include/wx4qt/wx/filename.h
    ../include/wx4qt/wx/messagedialog.h
    ../include/wx4qt/wx/hashmap.h
    ../include/wx4qt/wx/longlong.h
    ../include/wx4qt/wx/logger.h
    ../include/wx4qt/wx/regex.h
    ../include/wx4qt/wx/string.h
    ../include/wx4qt/wx/thread.h
    ../include/wx4qt/wx/tokenzr.h
    ../include/wx4qt/wx/variant.h
    ../include/wx4qt/wx/wx.h
    ../include/wx4qt/wx/wxprec.h
    ../include/wx4qt/wx/wxtype.h
)

# =============================================================================
# Define STL specific source files (wrappers implementation)
# =============================================================================
set(STL_SOURCE_FILES
    # Internal files for library
    ../src/wx4stl/implements.cpp
    ../src/wx4stl/dynlib.cpp
    ../src/wx4stl/file.cpp
    ../src/wx4stl/filename.cpp
    ../src/wx4stl/messagedialog.cpp
    ../src/wx4stl/regex.cpp
    ../src/wx4stl/thread.cpp
    ../src/wx4stl/tokenzr.cpp
    # File that should be exported
    ../src/wx4stl/buffer.cpp
    ../src/wx4stl/confbase.cpp
    ../src/wx4stl/datetime.cpp
    ../src/wx4stl/dynarray.cpp
    ../src/wx4stl/logger.cpp
    ../src/wx4stl/longlong.cpp
    ../src/wx4stl/string.cpp
    ../src/wx4stl/variant.cpp
)

# =============================================================================
# Define STL specific header files (wrappers)
# =============================================================================
set(STL_HEADER_FILES
    ../include/wx4stl/wx/arrstr.h
    ../include/wx4stl/wx/buffer.h
    ../include/wx4stl/wx/confbase.h
    ../include/wx4stl/wx/datetime.h
    ../include/wx4stl/wx/dynarray.h
    ../include/wx4stl/wx/dynlib.h
    ../include/wx4stl/wx/file.h
    ../include/wx4stl/wx/filename.h
    ../include/wx4stl/wx/messagedialog.h
    ../include/wx4stl/wx/hashmap.h
    ../include/wx4stl/wx/longlong.h
    ../include/wx4stl/wx/logger.h
    ../include/wx4stl/wx/regex.h
    ../include/wx4stl/wx/string.h
    ../include/wx4stl/wx/thread.h
    ../include/wx4stl/wx/tokenzr.h
    ../include/wx4stl/wx/variant.h
    ../include/wx4stl/wx/wx.h
    ../include/wx4stl/wx/wxprec.h
    ../include/wx4stl/wx/wxtype.h
    
)

# =============================================================================
# Define resource file
# =============================================================================
set(MODULE_RC_FILE
    ../src/ETKSQLite3/resource.rc
)

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_WIN32_WINNT=0x0601)

    # Active hot reload for MSVC compilers if supported.
    if (POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo,MinSizeRel>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo,MinSizeRel>:ProgramDatabase>>")
    endif()

    # Add specific options to each configuration
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

        # =============================================================================
        # Define specific sources and headers files
        # =============================================================================
        if(framework STREQUAL "wxETKSQLite3")
            set(ETKSQLITE3_SOURCES ${COMMON_SOURCE_FILES})
            set(ETKSQLITE3_HEADERS ${COMMON_HEADER_FILES})
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
            set(ETKSQLITE3_SOURCES ${COMMON_SOURCE_FILES} ${QT_SOURCE_FILES})
            set(ETKSQLITE3_HEADERS ${COMMON_HEADER_FILES} ${QT_HEADER_FILES})
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

        source_group(
            TREE ${CMAKE_CURRENT_SOURCE_DIR}/..
            PREFIX "Source Files"
            FILES ${ETKSQLITE3_SOURCES}
        )

        source_group(
            TREE ${CMAKE_CURRENT_SOURCE_DIR}/..
            PREFIX "Header Files"
            FILES ${ETKSQLITE3_HEADERS}
        )

        source_group(
            TREE ${CMAKE_CURRENT_SOURCE_DIR}/..
            PREFIX "Resource Files"
            FILES ${MODULE_RC_FILE}
        )

        # Create library
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
        # =============================================================================
        # Define specific sources and headers files
        # =============================================================================
        if(framework STREQUAL "wxETKSQLite3")
            target_include_directories(${LIB_NAME} PUBLIC $(WX_INCLUDE_DIRS) ../include/ETKSQLite3 ${WXSQLITE3_DIR}/include ${WXSQLITE3_DIR}/src)
        elseif(framework STREQUAL "qtETKSQLite3")
            # Include directories
            #target_include_directories(${LIB_NAME} PUBLIC ../include/ETKSQLite3 ../include/wx4qt ${WXSQLITE3_DIR}/include ${WXSQLITE3_DIR}/src)
            target_include_directories(${LIB_NAME} PUBLIC ../include/ETKSQLite3 ../include/wx4qt ${WXSQLITE3_DIR}/include ${WXSQLITE3_DIR}/src ${QT_INCLUDE_DIRS} )
        elseif(framework STREQUAL "stlETKSQLite3")
            target_include_directories(${LIB_NAME} PUBLIC ../include/ETKSQLite3 ../include/wx4stl ${WXSQLITE3_DIR}/include ${WXSQLITE3_DIR}/src)
        else()
            message(FATAL_ERROR "No target type for ETKSQLite3 is found, ${framework} is selected but not supported. Error into CMake file!")
        endif()

        target_link_libraries(${LIB_NAME}      PRIVATE ${FRAMEWORK_LIBS})
        target_compile_definitions(${LIB_NAME} PRIVATE ${FRAMEWORK_DEFINITIONS})

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
                set(DESTINATION_DIR_BIN "${CMAKE_CURRENT_SOURCE_DIR}/bin/win32")
            else()
                # Define ouput folder for linux
                set(DESTINATION_DIR_BIN "${CMAKE_CURRENT_SOURCE_DIR}/bin/linux")
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
        endif()
        # Copy lib
        if (WIN32)
            # Define ouput folder for windows
            set(DESTINATION_DIR_LIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/win32")
        else()
            # Define ouput folder for linux
            set(DESTINATION_DIR_LIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/linux")
        endif()
        # Create destination folder if it doesn't exits
        add_custom_command(TARGET ${LIB_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E make_directory
                           "${DESTINATION_DIR_LIB}/$<CONFIG>/")
        # Copy .lib or .a files for dynamic / statics libs
        add_custom_command(TARGET ${LIB_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy
                           "$<TARGET_FILE:${LIB_NAME}>"
                           "${DESTINATION_DIR_LIB}/$<CONFIG>/")

        # =============================================================================
        # Intallation
        # =============================================================================
        # Check the target exists
        if(NOT TARGET ${LIB_NAME})
            message(FATAL_ERROR "Target ${LIB_NAME} does not exist. Check your CMakeLists.txt.")
        endif()
        # And make the install
        install(TARGETS ${LIB_NAME}
                ARCHIVE DESTINATION lib
                LIBRARY DESTINATION lib
                PUBLIC_HEADER DESTINATION include/${framework}
        )
    endif()
endfunction()

# =============================================================================
# Add Target intall
# =============================================================================
add_custom_target(Installation ALL
    COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --config $<CONFIGURATION>
    COMMENT "Installing ETKSQLite3...")

# =============================================================================
# Create library depending of options
# =============================================================================
create_framework_library(wxETKSQLite3 "wx")
create_framework_library(qtETKSQLite3 "qt")
create_framework_library(stlETKSQLite3 "stl")

# =============================================================================
# Check options of ETKSQLite3 generation type
# =============================================================================
if(NOT wxETKSQLite3 AND NOT qtETKSQLite3 AND NOT stlETKSQLite3)
    message(FATAL_ERROR "No target type for ETKSQLite3 is selected. You must activate at least wxETKSQLite3, qtETKSQLite3 or boostETKSQLite3 or stlETKSQLite3.")
endif()

# =============================================================================
# Check if wxSQLite3 directory exists
# =============================================================================
if(NOT EXISTS "${WXSQLITE3_DIR}")
    message(FATAL_ERROR "The specified wxSQLite3 directory does not exist: ${WXSQLITE3_DIR}. Please set WXSQLITE3_DIR to a valid path, this path is mandatory!")
endif()