@ECHO OFF

ECHO Build wxWidgets with MinGW

set SCRIPT_DIR=%~dp0

CD /D "%SCRIPT_DIR%"

:: RELEASE
cmake -S "%SCRIPT_DIR%.." -B "%SCRIPT_DIR%build-release" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="%SCRIPT_DIR%install" -DwxBUILD_SHARED=OFF -DwxBUILD_SAMPLES=OFF -DwxBUILD_TESTS=OFF -DwxBUILD_MONOLITHIC=ON

mingw32-make -C "%SCRIPT_DIR%build-release" -j8
mingw32-make -C "%SCRIPT_DIR%build-release" install

:: DEBUG
cmake -S "%SCRIPT_DIR%.." -B "%SCRIPT_DIR%build-debug" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="%SCRIPT_DIR%install" -DwxBUILD_SHARED=OFF -DwxBUILD_SAMPLES=OFF -DwxBUILD_TESTS=OFF -DwxBUILD_MONOLITHIC=ON

mingw32-make -C "%SCRIPT_DIR%build-debug" -j8
mingw32-make -C "%SCRIPT_DIR%build-debug" install

ECHO Done.
