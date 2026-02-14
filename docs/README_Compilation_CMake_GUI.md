# Getting started: ETKSQLite3 installation and compilation with CMakeGUI
The **ETKSQLite3** use *CMake* to compile the library.<br/>

This will explain how to compile **ETKSQLite3** with *CMake* to compile it with any compiler you want.
*CMake* and *CMakeUI* (optional) must be installed on target to allow to generate makefles. 

## Download libraries
  - **ETKSQLite3**. You need to get **ETKSQLite3** library. Download using tag to get stable version.
    - Download [ETKSqlite3](https://github.com/Feneck91/ETKSQLite3) library from Github and decompress all source code in a folder (example **ETKSQLite3**).
    - You can also use git clone https://github.com/Feneck91/ETKSQLite3.git
  - **wxsqlite3** is a C++ wrapper around the public domain **SQLite 3.x** database and is specifically designed for use in programs based on the **wxWidgets** library. This library is written by **Ulrich Telle**.
    - Download [wxSQLite3](https://github.com/utelle/wxsqlite3) library from Github and decompress all source code in a folder (example **wxsqlite3**).
    - You can also use git clone https://github.com/utelle/wxsqlite3.git
  - **sqlite3** is the SQLite3 database. No need to download it, the sqlite3 amalgamation code is downloaded with **wxSQLite3** library.
      - But, If you want to see this library, you can alsa download it from [SQLite3](https://github.com/sqlite/sqlite)

**If you use git clone, you must switch the repository to correct branch / tag.**

## Run CMake

### Configure
Run the *CMakeGui*, you will ask to fill fields to begin the configure step.<br/>
The *CMakeLists.txt* that contains the informations to generate the library makefile and samples is into ETKSqlite3/CMakeLists.txt<br/>
<br/>
|               CMake               |             Fill (example)             |                                            Note                                             |
|-----------------------------------|----------------------------------------|---------------------------------------------------------------------------------------------|
| Where is the source code          | ETKSQLite3                             |  For example in <i>c:/dev/lib/ETKSQLite3</i>                                                |
| Where to build the binaries       | ETKSQLite3/build/Build.vc17            |  For example for Visual Studio 2022                                                         |
<br/>
Clic on Configure button, choose your compiler.<br/>

The first step is to define the **WXSQLITE3_DIR** variable. This variable is mandatory; it contains the source code path of wxsqlite3, base od the ETKSQLite3 framework.<br/>
If the wxsqlite3/ directory is located in the same directory as ETKSQLite3, it will be automatically detected. Otherwise, you will get the following error like:<br/>
<br/>
> WXSQLITE3_DIR not set and wxSQLite3 was not found next to ETKSQLite3.<br/>
> <br/>
> &nbsp;&nbsp;Expected layout:<br/>
> &nbsp;&nbsp;&nbsp;&nbsp;C:/dev/<br/>
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├─ ETKSQLite3/<br/>
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└─ wxsqlite3/<br/>
> <br/>
>  Or define WXSQLITE3_DIR manually.

Set **WXSQLITE3_DIR** if needed (not automatically detected).

|            CMake entry            | Entry type |                 Value                 |                                            Note                                             |
|-----------------------------------|------------|---------------------------------------|---------------------------------------------------------------------------------------------|
| WXSQLITE3_DIR                     | PATH       | ETKSQLite3/wxsqlite3                  | For example but where you download wxSQLite3 library                                        |

Clic on Configure button.<br/>
<br/>
> CMake Error at CMakeLists.txt (message):<br/>
> No target type for ETKSQLite3 is selected. You must activate at least<br/>
> ETK_ENABLE_WX, ETK_ENABLE_QT or ETK_ENABLE_STL.<br/>

You must choose the target:
- **wxETKSQLite3** for ETKSQLite3 based on wxWidgets (**ETK_ENABLE_WX** entry)<br/>
  If wxWidgets is not detected, you will probably need to add wxWidgets_ROOT_DIR path; you will have this error:<br/>

  > wxWidgets_ROOT_DIR not set and wxWidgets install was not found next to ETKSQLite3.<br/>
  > Expected layout for automatic detection:<br/>
  > &nbsp;&nbsp;C:/local/<br/>
  > &nbsp;&nbsp;&nbsp;&nbsp;├─ ETKSQLite3/<br/>
  > &nbsp;&nbsp;&nbsp;&nbsp;└─ wxWidgets-x.y.z/<br/>
  > &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└─ <build_folder>/<br/>
  > &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└─ install/<br/>
  > <br/>
  > Or define wxWidgets_ROOT_DIR manually to install wxWidgets folder.<br/>

  By default, **ETKSQLite3** will be statically linked to wxWidgets if it has been statically compiled.<br/>
  By default, **ETKSQLite3** will be dynamically linked to wxWidgets if it has been dynamic compiled.<br/>
  You can change this with **wxWidgets_WX_FORCE_DLL** and **wxWidgets_WX_FORCE_LIB**.<br/>
  
  |            CMake entry            | Entry type |                 Value                 |                                            Note                                             |
  |-----------------------------------|------------|---------------------------------------|---------------------------------------------------------------------------------------------|
  | wxWidgets_ROOT_DIR                | PATH       | C:/dev/wxWidgets-3.1.1/build/install  | Where wxWidgets is installed (/install folder)                                              |
  | wxWidgets_WX_FORCE_DLL            | PATH       | ON / OFF                              | Force **ETKSQLite3** to be linked as dynamic library mode to wxWidgets                      |
  | wxWidgets_WX_FORCE_LIB            | PATH       | ON / OFF                              | Force **ETKSQLite3** to be linked as static library mode to wxWidgets                       |
  
- **qtETKSQLite3** for ETKSQLite3 based on Qt (**ETK_ENABLE_QT** entry)
  If Qt is not detected, you will probably need to add CMAKE_INSTALL_PREFIX path.<br/>
  
  |            CMake entry            | Entry type |                 Value                 |                                            Note                                             |
  |-----------------------------------|------------|---------------------------------------|---------------------------------------------------------------------------------------------|
  | CMAKE_INSTALL_PREFIX              | PATH       | C:/Qt6/6.10.1/msvc2022_64             | Where QT is installed for wanted target  <i>C:/Qt6/6.10.1/msvc2022_64 or mingw_64</i>       |
- **stlETKSQLite3** for ETKSQLite3 based on C++ STL (**ETK_ENABLE_STL** entry)<br/>

Before Configure again, you should choice more options:<br/>

|            CMake entry            | Entry type |                 Value                 |                                 Note                                |
|-----------------------------------|------------|---------------------------------------|---------------------------------------------------------------------|
| SQLITE_ENABLE_*                   | BOOL       | Checked or not                        | See SQLite documentation to compile it with needed options.         |
| BUILD_SHARED_LIBS                 | BOOL       | Checked or not                        | If checked, build dynamic library (so / dll) else build static lib. |

## Generate 
Clic on ore **Generate** button. You should have something like:<br/>

> Selecting Windows SDK version 10.0.22621.0 to target Windows 10.0.26100.<br/>
> CMAKE_INSTALL_PREFIX = C:/local/ETKSQLite3/build.vc17/install<br/>
> Library is SHARED: allowing wxWidgets DLL build<br/>
> Detected wxWidgets Release lib: C:/dev/wxWidgets-3.3.1/build.vc17/install/lib/vc_x64_dll/wxmsw33u.lib<br/>
> Detected wxWidgets Debug lib: C:/dev/wxWidgets-3.3.1/build.vc17/install/lib/vc_x64_dll/wxmsw33ud.lib<br/>
> wxWidgets detected in MONOLITHIC mode<br/>
> Detected wxWidgets Release dynamic library: C:/dev/wxWidgets-3.3.1/build.vc17/install/bin/vc_x64_dll/wxmsw331u_vc_x64_custom.dll<br/>
> Detected wxWidgets Debug dynamic library: C:/dev/wxWidgets-3.3.1/build.vc17/install/bin/vc_x64_dll/wxmsw331ud_vc_x64_custom.dll<br/>
> Create shared library wxETKSQLite3<br/>
> Using Qt5<br/>
> Create shared library qtETKSQLite3<br/>
> Create shared library stlETKSQLite3<br/>
> ETKSQLite3 configured successfully<br/>
> Configuring done (0.2s)<br/>

# Open Project
From here, the files needed to build ETKSQLite3 are correctly generated.<br/>
Clic on ore **Open Project** button. Visual Studio opened (if you choose this target).<br/>
You can compile in debug / realase.
You **MUST** compile **INSTALL** projet to be able to compile client that will need to use *find_package* for **ETKSQLite3**.
