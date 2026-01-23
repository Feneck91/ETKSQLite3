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
  - **sqlite3** is the **sqlite3** database.
      - Download [SQLite3](https://github.com/sqlite/sqlite)

## Run CMake

### Configure
Run the *CMakeGui*, you will ask to fill fields to begin the configure step.<br/>
The *CMakeLists.txt* that contains the informations to generate the library makefile is into ETKSqlite3/build/CMakeLists.txt<br/>
The *CMakeLists.txt* that contains the informations to generate the library sample makefile is into ETKSqlite3/build/CMakeLists.txt<br/>
<br/>
|               CMake               |             Fill (example)             |                                           Comment                                           |
|-----------------------------------|----------------------------------------|---------------------------------------------------------------------------------------------|
| Where is the source code          | ETKSqlite3                             |  For example in <i>c:/dev/lib/ETKSqlite3</i>                                                |
| Where to build the binaries       | ETKSqlite3/build/Build.vc17            |  For example for Visual Studio 2022                                                         |
<br/>
Clic on Configure button.


## Generate 

# CMake

# Run CMake gui
