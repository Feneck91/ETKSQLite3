cmake_minimum_required(VERSION 3.29...3.31)
project(qtETKSQLite3Sample)

# ============================================================
# Build options
# ============================================================
option(USE_QT6                      "Use Qt6 instead of Qt5"                        ON)     # If QT5 / QT6 are detected, allow to use Qt6 instead of Qt5

# =============================================================================
# Find ETKSQLite3 package
# =============================================================================
# Cherche la config générée par l'installation d'ETKSQLite3
# Tu peux ajouter un hint vers ton build/install si nécessaire
#set(ETKSQLite3_DIR "C:/local/ETKSQLite3/build.vc17/install/lib/cmake/ETKSQLite3")
set(ETKSQLite3_DIR "C:/local/ETKSQLite3/build.vc17/install")
find_package(ETKSQLite3 REQUIRED CONFIG PATHS
    "${CMAKE_CURRENT_SOURCE_DIR}/../../build" # build tree
    "${CMAKE_CURRENT_SOURCE_DIR}/../../install" # install tree
    NO_DEFAULT_PATH
)

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

# AutoMoc on
set(CMAKE_AUTOMOC ON)

# =============================================================================
# Generate ws3 files
# =============================================================================
ETKSQLite3_generate_ws3("${CMAKE_CURRENT_SOURCE_DIR}/Database.ws3")

# =============================================================================
# Files
# =============================================================================
# Define common headers files
set(HEADER_FILES
    ETKSQlite3SampleDatabase.h
    qtetksqlite3sample.h
)

# Define common source files
set(SOURCE_FILES
    ETKSQlite3SampleDatabase.cpp
    qtetksqlite3sample.cpp
    main.cpp
)

# Define database files
set(DATABASE_FILES
    Database.ws3
)

set(RESOURCE_FILES
    qtetksqlite3sample.qrc
)

set(UI_FILES
    qtetksqlite3sample.ui
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    PREFIX "Source Files"
    FILES ${SOURCE_FILES}
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    PREFIX "Header Files"
    FILES ${HEADER_FILES}
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    PREFIX "Database Files"
    FILES ${DATABASE_FILES}
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    PREFIX "Resource Files"
    FILES ${RESOURCE_FILES}
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    PREFIX "ui Files"
    FILES ${UI_FILES}
)

source_group(
    TREE ${GENERATED_WS3_DIR}
    PREFIX "ws3 Generated Files"
    FILES
        ${GENERATED_WS3_SOURCES}
        ${GENERATED_WS3_HEADERS}
)

# =============================================================================
# Generate files .h from .ui
# =============================================================================

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

source_group(
    TREE ${CMAKE_CURRENT_BINARY_DIR}
    PREFIX "ui Generated Files"
    FILES ${UI_HEADERS}
)

# =============================================================================
# Build
# =============================================================================
add_definitions(-DUSING_ETK_SQLITE3_LIB)

# Add the executable
add_executable(qtETKSQLite3Sample ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} ${UI_FILES} ${DATABASE_FILES} ${UI_HEADERS} ${GENERATED_WS3_SOURCES} ${GENERATED_WS3_HEADERS})
target_sources(qtETKSQLite3Sample PRIVATE "${GENERATED_WS3_FILES}")

# =============================================================================
# Include directories
# =============================================================================
# ETKSQLite3::Qt is an IMPORTED target, use it to retrieve includes automatically
target_include_directories(qtETKSQLite3Sample PRIVATE $<TARGET_PROPERTY:ETKSQLite3::Qt,INTERFACE_INCLUDE_DIRECTORIES> "${CMAKE_CURRENT_BINARY_DIR}" "${GENERATED_WS3_DIR}")

# =============================================================================
# Link libraries
# =============================================================================
target_link_libraries(qtETKSQLite3Sample PRIVATE ETKSQLite3::Qt ${QT_LIBS})

# Compile definitions
target_compile_definitions(qtETKSQLite3Sample PRIVATE ETK_SQLITE3_USE_QT)
