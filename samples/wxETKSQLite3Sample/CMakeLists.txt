cmake_minimum_required(VERSION 3.29...3.31)
project(wxETKSQLite3Sample)

# =============================================================================
# Find ETKSQLite3 package
# =============================================================================
find_package(ETKSQLite3 REQUIRED CONFIG COMPONENTS WX)

# =============================================================================
# wxWidgets detection
# =============================================================================
ETKSQLite3_find_wxwidgets_auto(ROOT_DIR ${wxWidgets_ROOT_DIR}
                               LIB_DIR  ${wxWidgets_LIB_DIR}
                               WX_FORCE_DLL ON
)

# =============================================================================
# Generate ws3 files
# =============================================================================
ETKSQLite3_generate_ws3("${CMAKE_CURRENT_SOURCE_DIR}/wxETKSQLite3SampleDatabaseDatas.ws3")

# =============================================================================
# Files
# =============================================================================
# Define common headers files
set(HEADER_FILES
    wxETKSQLite3SampleApp.h
    wxETKSQLite3SampleMain.h
    wx_pch.h
)

# Define common source files
set(SOURCE_FILES
    wxETKSQLite3SampleApp.cpp
    wxETKSQLite3SampleMain.cpp
)

# Define database files
set(DATABASE_FILES
    wxETKSQLite3SampleDatabaseDatas.ws3
)

set(RESOURCE_FILES
    resource.rc
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
    TREE ${GENERATED_WS3_DIR}
    PREFIX "ws3 Generated Files"
    FILES
        ${GENERATED_WS3_SOURCES}
        ${GENERATED_WS3_HEADERS}
)

# =============================================================================
# Generate files .h from .ui
# =============================================================================
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
add_executable(wxETKSQLite3Sample WIN32 ${SOURCE_FILES} ${HEADER_FILES}  ${RESOURCE_FILES} ${UI_FILES} ${DATABASE_FILES} ${UI_HEADERS} ${GENERATED_WS3_SOURCES} ${GENERATED_WS3_HEADERS})
target_sources(wxETKSQLite3Sample PRIVATE "${GENERATED_WS3_FILES}")

# Précompiled header
target_precompile_headers(wxETKSQLite3Sample PRIVATE wx_pch.h)

# =============================================================================
# Include directories
# =============================================================================
target_include_directories(wxETKSQLite3Sample PRIVATE "${CMAKE_CURRENT_BINARY_DIR}" "${GENERATED_WS3_DIR}")

# =============================================================================
# Link libraries
# =============================================================================
target_link_libraries(wxETKSQLite3Sample PRIVATE ETKSQLite3::wxETKSQLite3 wxWidgets::wxWidgets)

# Compile definitions
target_compile_definitions(wxETKSQLite3Sample PRIVATE ETK_SQLITE3_USE_WX)

# Only copies DLLs if ETKSQLite3::wxETKSQLite3 is a shared library
ETKSQLite3_Copy_Dynamic_Libraries(wxETKSQLite3Sample ETKSQLite3::wxETKSQLite3)

# Copy wxWidgets DLLs
if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET wxETKSQLite3Sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying wxWidgets DLLs for $<CONFIG>"
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:wxETKSQLite3Sample>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<$<CONFIG:Debug>:${wxWidgets_DLLS_DEBUG}>
            $<$<CONFIG:Release>:${wxWidgets_DLLS_RELEASE}>
            $<$<CONFIG:RelWithDebInfo>:${wxWidgets_DLLS_RELEASE}>
            $<$<CONFIG:MinSizeRel>:${wxWidgets_DLLS_RELEASE}>
            $<TARGET_FILE_DIR:wxETKSQLite3Sample>
    )
endif()
