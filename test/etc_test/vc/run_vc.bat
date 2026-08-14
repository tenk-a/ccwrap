@echo off
rem  Thin wrapper for the MSVC etc_test (the tests that are NOT per-header). The build
rem  lives in CMake (CMakeLists.txt + CMakePresets.json in this folder); this only calls
rem  configure -> build -> test. A VS developer prompt is NOT needed -- the preset either
rem  uses a VS generator or goes through ..\..\bin\*_with_setcc.bat.
rem
rem  NOTE: keep this file ASCII-only (cmd.exe reads .bat in the OEM code page).
rem
rem    Usage:  run_vc.bat [preset]
rem      preset : vc145 (default) | vc143 | vc120-x86 | ...  (cmake --list-presets)
rem
rem    vc_c.c     C smoke: every C header includable, C11/C23 features work
rem    vc_cpp.cpp C++ smoke: the C headers usable from C++
rem  Both have a main(), so unlike the old script this RUNS them (not compile-only).
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
