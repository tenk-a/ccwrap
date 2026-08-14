@echo off
rem  Thin wrapper for the ccwrap test suites (Windows): configure -> build -> test.
rem  Everything else (per-compiler flags, include paths, forced include, the per-file
rem  compile probe) lives in CMake -- see test\CMakeLists.txt and testin\ccwrap_test.cmake.
rem
rem  NOTE: keep this file ASCII-only. cmd.exe reads .bat in the OEM code page and
rem  `goto` seeks by BYTE offset, so UTF-8 Japanese in comments derails label lookup.
rem
rem    Usage:  run.bat [preset] [extra cmake args...]
rem
rem      preset : watcom (default) | watcom-ccwstd | vc145 | vc143 | gcc | clang | ...
rem               list them with  cmake --list-presets
rem
rem    e.g.  run.bat                          all suites on Open Watcom
rem          run.bat vc145                    c_test + cxx_test + etc_test\vc on VS2026
rem          run.bat watcom --fresh           drop the cache (re-run the compile probe)
rem          run.bat watcom -DTST_ONLY=string one file only (may name any suite's file)
rem          run.bat watcom -DTST_SUITES=c_test   pick the suites
rem
rem  TST_SUITES / TST_ONLY are CACHE variables: once set in a build dir they stay set.
rem  Pass -DTST_SUITES=auto (or --fresh) to go back to "every suite for this compiler".
rem
rem  To run a single suite on its own, the same three steps work in its folder:
rem    cd c_test ^&^& cmake --preset watcom ^&^& cmake --build --preset watcom ^&^& ctest --preset watcom
setlocal
cd /d "%~dp0"

set "PRESET=%~1"
if "%PRESET%"=="" set "PRESET=watcom"
if not "%~1"=="" shift

rem  The remaining arguments (--fresh / -D...) go to the configure step only.
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
