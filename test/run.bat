@echo off
setlocal
cd /d "%~dp0"

set "PRESET=%~1"
if "%PRESET%"=="" set "PRESET=watcom"
if not "%~1"=="" shift

set "EXTRA="
:args
if "%~1"=="" goto configure
set "EXTRA=%EXTRA% %1"
shift
goto args

:configure
cmake --preset %PRESET%%EXTRA%
if errorlevel 1 ( echo CONFIGURE FAILED ^(%PRESET%^) & exit /b 1 )

cmake --build --preset %PRESET%
if errorlevel 1 ( echo BUILD FAILED ^(%PRESET%^) & exit /b 1 )

ctest --preset %PRESET%
exit /b %errorlevel%
