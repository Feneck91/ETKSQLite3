@ECHO OFF
:: Copy this file into wxWidgets root folder like wxWidgets.x.y.z/build.mingw (or other sub folder)
:: You can change the compilation options
:: All build are monolithic
:: Both Debug / Release are compiled
:: Both unicode or not are compiled

ECHO Build wxWidgets with MinGW

set SCRIPT_DIR=%~dp0

CD /D "%SCRIPT_DIR%"

::
:: Build Static
::

:: RELEASE
ECHO ===================================================================
ECHO ==                        STATIC RELEASE                         ==
ECHO ===================================================================
cmake -S "%SCRIPT_DIR%.." -B "%SCRIPT_DIR%build-static-release" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="%SCRIPT_DIR%install" -DwxUSE_STL=ON -DwxUSE_UNICODE=OFF -DwxBUILD_SHARED=OFF -DwxBUILD_SAMPLES=OFF -DwxBUILD_TESTS=OFF -DwxBUILD_MONOLITHIC=ON

mingw32-make -C "%SCRIPT_DIR%build-release" -j8
mingw32-make -C "%SCRIPT_DIR%build-release" install

:: RELEASE UNICODE
ECHO ===================================================================
ECHO ==                     STATIC RELEASE UNICODE                    ==
ECHO ===================================================================
cmake -S "%SCRIPT_DIR%.." -B "%SCRIPT_DIR%build-static-release-unicode" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="%SCRIPT_DIR%install" -DwxUSE_STL=ON -DwxUSE_UNICODE=ON -DwxBUILD_SHARED=OFF -DwxBUILD_SAMPLES=OFF -DwxBUILD_TESTS=OFF -DwxBUILD_MONOLITHIC=ON

mingw32-make -C "%SCRIPT_DIR%build-release" -j8
mingw32-make -C "%SCRIPT_DIR%build-release" install

:: DEBUG
ECHO ===================================================================
ECHO ==                         STATIC DEBUG                          ==
ECHO ===================================================================
cmake -S "%SCRIPT_DIR%.." -B "%SCRIPT_DIR%build-static-debug" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="%SCRIPT_DIR%install" -DwxUSE_STL=ON -DwxUSE_UNICODE=OFF -DwxBUILD_SHARED=OFF -DwxBUILD_SAMPLES=OFF -DwxBUILD_TESTS=OFF -DwxBUILD_MONOLITHIC=ON

mingw32-make -C "%SCRIPT_DIR%build-debug" -j8
mingw32-make -C "%SCRIPT_DIR%build-debug" install

:: DEBUG UNICODE
ECHO ===================================================================
ECHO ==                      STATIC DEBUG UNICODE                     ==
ECHO ===================================================================
cmake -S "%SCRIPT_DIR%.." -B "%SCRIPT_DIR%build-static-debug-unicode" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="%SCRIPT_DIR%install" -DwxUSE_STL=ON -DwxUSE_UNICODE=ON -DwxBUILD_SHARED=OFF -DwxBUILD_SAMPLES=OFF -DwxBUILD_TESTS=OFF -DwxBUILD_MONOLITHIC=ON

mingw32-make -C "%SCRIPT_DIR%build-debug" -j8
mingw32-make -C "%SCRIPT_DIR%build-debug" install

::
:: Build Dynamic
::

:: RELEASE
ECHO ===================================================================
ECHO ==                       DYNAMIC RELEASE                         ==
ECHO ===================================================================
cmake -S "%SCRIPT_DIR%.." -B "%SCRIPT_DIR%build-release" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="%SCRIPT_DIR%install" -DwxUSE_STL=ON -DwxUSE_UNICODE=OFF -DwxBUILD_SHARED=ON -DwxBUILD_SAMPLES=OFF -DwxBUILD_TESTS=OFF -DwxBUILD_MONOLITHIC=ON

mingw32-make -C "%SCRIPT_DIR%build-release" -j8
mingw32-make -C "%SCRIPT_DIR%build-release" install

:: RELEASE UNICODE
ECHO ===================================================================
ECHO ==                    DYNAMIC RELEASE UNICODE                    ==
ECHO ===================================================================
cmake -S "%SCRIPT_DIR%.." -B "%SCRIPT_DIR%build-release-unicode" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="%SCRIPT_DIR%install" -DwxUSE_STL=ON -DwxUSE_UNICODE=ON -DwxBUILD_SHARED=ON -DwxBUILD_SAMPLES=OFF -DwxBUILD_TESTS=OFF -DwxBUILD_MONOLITHIC=ON

mingw32-make -C "%SCRIPT_DIR%build-release" -j8
mingw32-make -C "%SCRIPT_DIR%build-release" install

:: DEBUG
ECHO ===================================================================
ECHO ==                        DYNAMIC DEBUG                          ==
ECHO ===================================================================
cmake -S "%SCRIPT_DIR%.." -B "%SCRIPT_DIR%build-debug" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="%SCRIPT_DIR%install" -DwxUSE_STL=ON -DwxUSE_UNICODE=OFF -DwxBUILD_SHARED=ON -DwxBUILD_SAMPLES=OFF -DwxBUILD_TESTS=OFF -DwxBUILD_MONOLITHIC=ON

mingw32-make -C "%SCRIPT_DIR%build-debug" -j8
mingw32-make -C "%SCRIPT_DIR%build-debug" install

:: DEBUG UNICODE
ECHO ===================================================================
ECHO ==                     DYNAMIC DEBUG UNICODE                     ==
ECHO ===================================================================
cmake -S "%SCRIPT_DIR%.." -B "%SCRIPT_DIR%build-debug-unicode" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="%SCRIPT_DIR%install" -DwxUSE_STL=ON -DwxUSE_UNICODE=ON -DwxBUILD_SHARED=ON -DwxBUILD_SAMPLES=OFF -DwxBUILD_TESTS=OFF -DwxBUILD_MONOLITHIC=ON

mingw32-make -C "%SCRIPT_DIR%build-debug" -j8
mingw32-make -C "%SCRIPT_DIR%build-debug" install

ECHO Done.
