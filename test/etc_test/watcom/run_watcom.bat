@echo off
setlocal
cd /d "%~dp0"

cmake --preset watcom %*
if errorlevel 1 ( echo CONFIGURE FAILED & exit /b 1 )
cmake --build --preset watcom
if errorlevel 1 ( echo BUILD FAILED & exit /b 1 )
ctest --preset watcom
exit /b %ERRORLEVEL%
