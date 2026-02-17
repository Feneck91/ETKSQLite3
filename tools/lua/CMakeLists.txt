cmake_minimum_required(VERSION 3.16)
project(ETKSQLite3Lua C)

set(ETKSQLITE3_LUA_HEADERS
    include/lapi.h
    include/lauxlib.h
    include/lcode.h
    include/lctype.h
    include/ldebug.h
    include/ldo.h
    include/lfunc.h
    include/lgc.h
    include/ljumptab.h
    include/llex.h
    include/llimits.h
    include/lmem.h
    include/lobject.h
    include/lopcodes.h
    include/lopnames.h
    include/lparser.h
    include/lprefix.h
    include/lstate.h
    include/lstring.h
    include/ltable.h
    include/ltests.h
    include/ltm.h
    include/lua.h
    include/luaconf.h
    include/lualib.h
    include/lundump.h
    include/lvm.h
    include/lzio.h
)

set(ETKSQLITE3_LUA_SOURCES
    src/lapi.c
    src/lauxlib.c
    src/lbaselib.c
    src/lcode.c
    src/lcorolib.c
    src/lctype.c
    src/ldblib.c
    src/ldebug.c
    src/ldo.c
    src/ldump.c
    src/lfunc.c
    src/lgc.c
    src/linit.c
    src/liolib.c
    src/llex.c
    src/lmathlib.c
    src/lmem.c
    src/loadlib.c
    src/lobject.c
    src/lopcodes.c
    src/loslib.c
    src/lparser.c
    src/lstate.c
    src/lstring.c
    src/lstrlib.c
    src/ltable.c
    src/ltablib.c
    src/ltests.c
    src/ltm.c
    src/lua.c
    src/lundump.c
    src/lutf8lib.c
    src/lvm.c
    src/lzio.c
)

# Create source groups for IDE
source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PREFIX "Source Files"
    FILES ${ETKSQLITE3_LUA_SOURCES}
)
source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PREFIX "Header Files"
    FILES ${ETKSQLITE3_LUA_HEADERS}
)
add_executable(ETKSQLite3Lua ${ETKSQLITE3_LUA_HEADERS} ${ETKSQLITE3_LUA_SOURCES})
target_include_directories(ETKSQLite3Lua PRIVATE include)

if(UNIX)
    target_link_libraries(ETKSQLite3Lua PRIVATE m dl)
endif()
