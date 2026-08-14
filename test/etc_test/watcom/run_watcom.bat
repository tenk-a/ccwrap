@echo off
rem  Thin wrapper for the Open Watcom etc_test (the tests that are NOT per-header).
rem  The build lives in CMake (CMakeLists.txt + CMakePresets.json in this folder);
rem  this only calls configure -> build -> test.
rem
rem  NOTE: keep this file ASCII-only (cmd.exe reads .bat in the OEM code page).
rem
rem    Usage:  run_watcom.bat [extra cmake args...]
rem      e.g.  run_watcom.bat                        everything
rem            run_watcom.bat -DTST_ONLY=watcom_c99  one program only
rem            run_watcom.bat --fresh                drop the cache
rem
rem  The Watcom location comes from the preset (WATCOM=C:/WATCOM). To use another
rem  tree, edit CMakePresets.json or drop in a CMakeUserPresets.json.
rem
rem  What it covers:
rem    watcom_ctest     C smoke (Win32)
rem    watcom_cinclude  every C header must be includable from C
rem    watcom_cmath     the C99 f/l math names exist for C (not just <cmath>)
rem    watcom_c99       the C99-only surface (<fenv.h>), built with -za99
rem    watcom_stdbit    <stdbit.h> type-generic macros from C
rem    intrin           watcom/intrin.h intrinsics (a C++ framework test)
setlocal
cd /d "%~dp0"

cmake --preset watcom %*
if errorlevel 1 ( echo CONFIGURE FAILED & exit /b 1 )
cmake --build --preset watcom
if errorlevel 1 ( echo BUILD FAILED & exit /b 1 )
ctest --preset watcom
exit /b %ERRORLEVEL%
