@echo off
setlocal
cd /d "%~dp0"

set "PRESET=%~1"
if "%PRESET%"=="" set "PRESET=vc145"

cmake --preset %PRESET%
if errorlevel 1 ( echo CONFIGURE FAILED & exit /b 1 )
cmake --build --preset %PRESET%
if errorlevel 1 ( echo BUILD FAILED & exit /b 1 )
ctest --preset %PRESET%
exit /b %ERRORLEVEL%
