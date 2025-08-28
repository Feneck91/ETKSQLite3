cmake_minimum_required(VERSION 3.29...3.31)
project(qtETKSQLite3Sample)

# =============================================================================
# Ask to user the path where wxsqlite3 is installed
# =============================================================================
set(WXSQLITE3_DIR           "../../wxsqlite3" CACHE PATH      "Installation path of wxsqlite3")

# =============================================================================
# Custom build step for <files>.ws3
# =============================================================================
function(generate_ws3 in_ws3)
    get_filename_component(base ${in_ws3} NAME_WE)
    set(out_cpp "${CMAKE_CURRENT_SOURCE_DIR}/${base}.cpp")
    set(out_h  "${CMAKE_CURRENT_SOURCE_DIR}/${base}.h")
    add_custom_command(
        OUTPUT  ${out_cpp}
                ${out_h}
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../../build/Win32/Lua52.exe"
                "${CMAKE_CURRENT_SOURCE_DIR}/../../scripts/compile_ws3.lua"
                "${in_ws3}" qt "${out_h}" "${out_cpp}"
        DEPENDS "${in_ws3}"
                "${CMAKE_CURRENT_SOURCE_DIR}/../../scripts/compile_ws3.lua"
        COMMENT "Compiling ws3 database ${in_ws3} -> ${out_cpp}, ${out_h}"
        VERBATIM
    )

    # Expose generated files to parent scope
    set(GENERATED_SOURCES ${GENERATED_SOURCES} ${out_cpp} PARENT_SCOPE)
    set(GENERATED_HEADERS ${GENERATED_HEADERS} ${out_h}   PARENT_SCOPE)
endfunction()

# =============================================================================
# Find Qt5
# =============================================================================
find_package(Qt5 REQUIRED COMPONENTS Widgets)
# AutoMoc on
set(CMAKE_AUTOMOC ON)

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
    Database.cpp
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

# =============================================================================
# Generate ws3 files
# =============================================================================
generate_ws3("${CMAKE_CURRENT_SOURCE_DIR}/Database.ws3")

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

# =============================================================================
# Generate files .h from .ui
# =============================================================================
set(UI_FILES
    qtetksqlite3sample.ui
)
qt5_wrap_ui(UI_HEADERS ${UI_FILES})

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    PREFIX "ui Generated Files"
    FILES ${UI_HEADERS}
)

# =============================================================================
# Find qtETKSQLite3
# =============================================================================
if (WIN32)
    # Define folder for windows
    set(FOLDER_OS "win32")
else()
    # Define folder for linux
    set(FOLDER_OS "linux")
endif()

# =============================================================================
# Build
# =============================================================================
add_definitions(-DUSING_ETK_SQLITE3_LIB)

link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../build/lib/${FOLDER_OS}/${CONFIGURATION}")
# Add the executable
add_executable(qtETKSQLite3Sample ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} ${UI_FILES} ${DATABASE_FILES} ${UI_HEADERS})

# Link to qtETKSQLite3 and Qt5
target_link_libraries(qtETKSQLite3Sample PRIVATE qtETKSQLite3_static.lib Qt5::Widgets)

# Include directories
target_include_directories(qtETKSQLite3Sample PRIVATE "../../include/ETKSQLite3" "../../include/wx4qt" "${WXSQLITE3_DIR}/include" "${WXSQLITE3_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}")

# Compile definitions
target_compile_definitions(qtETKSQLite3Sample PRIVATE ETK_SQLITE3_USE_QT)

# =============================================================================
# Check if wxSQLite3 directory exists
# =============================================================================
if(NOT EXISTS "${WXSQLITE3_DIR}")
    message(FATAL_ERROR "The specified wxSQLite3 directory does not exist: ${WXSQLITE3_DIR}. Please set WXSQLITE3_DIR to a valid path, this path is mandatory!")
endif()
