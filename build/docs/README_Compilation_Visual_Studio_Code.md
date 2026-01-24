# Getting started: ETKSQLite3 installation and compilation with Visual Studio Code
The **ETKSQLite3** use *CMake* to compile the library.<br/>

This will explain how to compile **ETKSQLite3** with *Visual Studio Code* to compile it with any compiler you want.
*Visual Studio Code* use at least 2 extensions that must be installed.
  - C/C++ (Microsoft)
  - CMake Tools (Microsoft)

## Download libraries
  - **ETKSQLite3**. You need to get **ETKSqlite3** library. Download using tag to get stable version.
    - Download [ETKSqlite3](https://github.com/Feneck91/ETKSqlite3) library from Github and decompress all source code in a folder (example **ETKSQLite3**).
    - You can also use git clone https://github.com/Feneck91/ETKSqlite3.git
  - **wxsqlite3** is a C++ wrapper around the public domain **SQLite 3.x** database and is specifically designed for use in programs based on the **wxWidgets** library. This library is written by **Ulrich Telle**.
    - Download [wxSQLite3](https://github.com/utelle/wxsqlite3) library from Github and decompress all source code in a folder (example **ETKSQLite3/wxSQLite3**).
    - You can also use git clone https://github.com/utelle/wxsqlite3.git
  - **sqlite3** is the SQLite3 database. No need to download it, the sqlite3 amalgamation code is downloaded with **wxSQLite3** library.
      - But, If you want to see this library, you can alsa download it from [SQLite3](https://github.com/sqlite/sqlite)

## Run Visual Studio Code

### Configure
  - Open the Folder **ETKSQLite3/build**.<br/>
  - Open CMake (Press **CTRL** + **SHIFT** + **P**).
    - Clic on **Open Workspace Settings**
    - If you want to change the output folder, search **Cmake: Build Directory** and change *${workspaceFolder}/build* to *${workspaceFolder}/Build.vc17* (or other like *${workspaceFolder}/mingw*) and press **ENTER**.
  - Open CMake (Press **CTRL** + **SHIFT** + **P**).
    - Clic on **CMake: Configure**, then an error occurs:
      > [cmake] No target type for ETKSQLite3 is selected.  You must activate at least<br/>
      > [cmake] wxETKSQLite3, qtETKSQLite3 or stlETKSQLite3.<br/>
    - If you have changed the build directory, you can now delete the old *${workspaceFolder}/build* folder.

### Fill configuration
Here, the configure failed but the file ETKSQLite3/build/[build you choose]/CMakeCache.txt is generated. You can now edit it.<br/>
Select and choose one target:<br/>

|                              CMakeCache.txt                                |           Fill (example)           |                                            Note                                             |
|----------------------------------------------------------------------------|------------------------------------|---------------------------------------------------------------------------------------------|
| WXSQLITE3_DIR:PATH=Path where wxsqlite3 is installed (mandatory)           | WXSQLITE3_DIR:PATH=../../wxSQLite3 | This is MANDATORY! For example but where you download wxSQLite3 library                     |
| wxETKSQLite3:BOOL=OFF                                                      | wxETKSQLite3:BOOL=ON               |  Activate compilation for ETKSQLite3 for wxWidgets                                          |
| qtETKSQLite3:BOOL=OFF                                                      | qtETKSQLite3:BOOL=ON               |  Activate compilation for ETKSQLite3 for Qt                                                 |
| stlETKSQLite3:BOOL=OFF                                                     | stlETKSQLite3:BOOL=ON              |  Activate compilation for ETKSQLite3 for STL                                                |

  - wxETKSQLite3 for ETKSQLite3 is choose
    > [cmake] CMake Error at CMakeLists.txt:361 (message):<br/>
    > [cmake]   find_wxwidgets_auto requires ROOT_DIR=<path to wxWidgets><br/>
    You must install wxWidgets
  - qtETKSQLite3 for ETKSQLite3 is choose
    > [cmake] CMake Error at CMakeLists.txt:92 (message):<br/>
    > [cmake]   Neither Qt5 nor Qt6 found.
    If Qt is not detected, you will probably need to add **CMAKE_INSTALL_PREFIX** path.<br/>

|                              CMakeCache.txt                                |                   Fill (example)                    |                                            Note                                             |
|----------------------------------------------------------------------------|-----------------------------------------------------|---------------------------------------------------------------------------------------------|
| CMAKE_INSTALL_PREFIX:PATH=C:/Program Files/ETKSQLite3                      | CMAKE_INSTALL_PREFIX:PATH=C:/Qt6/6.10.1/msvc2022_64 |	                        | Where QT is installed for wanted target C:/Qt6/6.10.1/msvc2022_64 or mingw_64               |

Open CMake (Press **CTRL** + **SHIFT** + **P**).
  - Clic on **CMake: Configure**, the vcxproj must be generated.<br/>
    > [main] Configuring project: build <br/>
    > [proc] Executing command: "C:\Program Files\CMake\bin\cmake.EXE" -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --no-warn-unused-cli -S C:/Dev/C++/ETKSqlite3/build -B c:/Dev/C++/ETKSqlite3/build/Build.vc17 -G "Visual Studio 17 2022" -T host=x86 -A x64<br/>
    > [cmake] Not searching for unused variables given on the command line.<br/>
    > [cmake] -- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.26200.<br/>
    > [cmake] -- Only Qt6 is installed. Using Qt6.<br/>
    > [cmake] CONFIGURATION: DEBUG MODE DETECTED = DEBUG<br/>
    > [cmake] CONFIGURATION: RELEASE MODE DETECTED = RELEASE<br/>
    > [cmake] CONFIGURATION: RELEASE MODE DETECTED = RELWITHDEBINFO<br/>
    > [cmake] CONFIGURATION: RELEASE MODE DETECTED = MINSIZEREL<br/>
    > [cmake] CONFIGURATION: DEBUG MODE DETECTED = DEBUG<br/>
    > [cmake] CONFIGURATION: RELEASE MODE DETECTED = RELEASE<br/>
    > [cmake] CONFIGURATION: RELEASE MODE DETECTED = RELWITHDEBINFO<br/>
    > [cmake] CONFIGURATION: RELEASE MODE DETECTED = MINSIZEREL<br/>
    > [cmake] -- Using Qt6<br/>
    > [cmake] Create shared library qtETKSQLite3<br/>
    > [cmake] -- Configuring done (0.4s)<br/>
    > [cmake] -- Generating done (0.2s)<br/>
    > [cmake] -- Build files have been written to: C:/Dev/C++/ETKSqlite3/build/Build.vc17<br/>
    
# Compile Project
From here, the files needed to build ETKSQLite3 are correctly generated.
