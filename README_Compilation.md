# Getting started: ETKSQLite3 installation and compilation
The **ETKSQLite3** use *CMake* to compile the library.<br/>

This will explain how to compile **ETKSQLite3** with *CMake* to compile it with any compiler you want.
*CMake* and *CMakeUI* (optional) must be installed on target to allow to generate makefles. 

## Download libraries
  - **ETKSQLite3**. You need to get **ETKSqlite3** library. Download using tag to get stable version.
    - Download [ETKSqlite3](https://github.com/Feneck91/ETKSqlite3) library from Github and decompress all source code in a folder (example **ETKSQLite3**).
    - You can also use git clone https://github.com/Feneck91/ETKSqlite3.git
  - **wxsqlite3** is a C++ wrapper around the public domain **SQLite 3.x** database and is specifically designed for use in programs based on the **wxWidgets** library. This library is written by **Ulrich Telle**.
    - Download [wxSQLite3](https://github.com/utelle/wxsqlite3) library from Github and decompress all source code in a folder (example **ETKSQLite3/wxSQLite3**).
    - You can also use git clone https://github.com/utelle/wxsqlite3.git
  - **sqlite3** is the SQLite3 database. No need to download it, the sqlite3 amalgamation code is downloaded with **wxSQLite3** library.
      - But, If you want to see this library, you can alsa download it from [SQLite3](https://github.com/sqlite/sqlite)

## Run CMake

### Configure
Run the *CMakeGui*, you will ask to fill fields to begin the configure step.<br/>
The *CMakeLists.txt* that contains the informations to generate the library makefile is into ETKSqlite3/build/CMakeLists.txt<br/>
The *CMakeLists.txt* that contains the informations to generate the library sample makefile is into ETKSqlite3/build/CMakeLists.txt<br/>
<br/>
|               CMake               |             Fill (example)             |                                            Note                                             |
|-----------------------------------|----------------------------------------|---------------------------------------------------------------------------------------------|
| Where is the source code          | ETKSqlite3                             |  For example in <i>c:/dev/lib/ETKSqlite3</i>                                                |
| Where to build the binaries       | ETKSqlite3/build/Build.vc17            |  For example for Visual Studio 2022                                                         |
<br/>
Clic on Configure button, choose your compiler, then an error occurs:

> CMake Error at CMakeLists.txt:636 (message):<br/>
> No target type for ETKSQLite3 is selected. You must activate at least<br/>
> wxETKSQLite3, qtETKSQLite3 or stlETKSQLite3.

You must choose the target:
- **wxETKSQLite3** for ETKSQLite3 based on wxWidgets
- **qtETKSQLite3** for ETKSQLite3 based on Qt
  If Qt is not detected, you will probably need to add CMAKE_INSTALL_PREFIX path.<br/>
  <br/>
  
  |            CMake entry            | Entry type |                 Value                 |                                            Note                                             |
  |-----------------------------------|------------|---------------------------------------|---------------------------------------------------------------------------------------------|
  | CMAKE_INSTALL_PREFIX              | PATH       | C:/Qt6/6.10.1/msvc2022_64             | Where QT is installed for wanted target  <i>C:/Qt6/6.10.1/msvc2022_64 or mingw_64</i>       |
  | WXSQLITE3_DIR                     | PATH       | ETKSQLite3/wxSQLite3                  | For example but where you download wxSQLite3 library                                        |
  
  <br/>
- **stlETKSQLite3** for ETKSQLite3 based on C++ STL
<br/>
Before Configure again, you should choice more options:<br/>
<br/>

|            CMake entry            | Entry type |                 Value                 |                                 Note                                |
|-----------------------------------|------------|---------------------------------------|---------------------------------------------------------------------|
| SQLITE_ENABLE_*                   | BOOL       | Checked or not                        | See SQLite documentation to compile it with needed options.         |
| SQLITE_ENABLE_COLUMN_METADATA     | BOOL       | Checked                               | This options is mandatory for ETKSQLite3.                           |
| BUILD_SHARED_LIBS                 | BOOL       | Checked                               | If checked, build dynamic library (so / dll) else build static lib. |

## Generate 
Clic on ore **Generate** button. You should have something like:<br/>

> Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.26200.<br/>
> Only Qt6 is installed. Using Qt6.<br/>
> CONFIGURATION: DEBUG MODE DETECTED = DEBUG<br/>
> CONFIGURATION: RELEASE MODE DETECTED = RELEASE<br/>
> CONFIGURATION: RELEASE MODE DETECTED = RELWITHDEBINFO<br/>
> CONFIGURATION: RELEASE MODE DETECTED = MINSIZEREL<br/>
> CONFIGURATION: DEBUG MODE DETECTED = DEBUG<br/>
> CONFIGURATION: RELEASE MODE DETECTED = RELEASE<br/>
> CONFIGURATION: RELEASE MODE DETECTED = RELWITHDEBINFO<br/>
> CONFIGURATION: RELEASE MODE DETECTED = MINSIZEREL<br/>
> Using Qt6<br/>
> Create static library qtETKSQLite3<br/>
> Configuring done (0.4s)<br/>
> Generating done (0.2s)<br/>

# Open Project
From here, the files needed to build ETKSQLite3 are correctly generated.
Clic on ore **Open Project** button. Visual Studio opened (if you choose this target).<br/>
You can compile in debug / realase.
