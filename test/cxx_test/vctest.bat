@echo off
rem  Thin wrapper: run the per-header C++ suite on Visual C++. The build lives in CMake
rem  (CMakeLists.txt + CMakePresets.json in this folder); this only maps the old arguments
rem  onto a preset and calls configure -> build -> test.
rem
rem  NOTE: keep this file ASCII-only. cmd.exe reads .bat in the OEM code page and `goto`
rem  seeks by BYTE offset, so UTF-8 Japanese in comments derails label lookup.
rem
rem    Usage:  vctest.bat [std] [c++NN] [vcNNN] [x64|x86]
rem
rem      std      -> build against the compiler's OWN standard library (baseline).
rem                  Anything that fails there is a bug in the TEST, not in ccwrap.
rem                  (= -DTEST_TARGET_CXX= , i.e. no ccwrap target level at all)
rem      c++NN    -> language level c++14 / c++17 / c++20 / c++latest (default latest).
rem                  (= -DTST_MSVC_STD=c++NN)
rem                  NB: VC 14.5 has no /std:c++23 -- it silently falls back to C++14.
rem      vcNNN    -> toolchain (vc145 / vc143 / vc120 / ...). Default vc145.
rem                  vc145/vc143 x64 use a VS generator preset; anything else uses the
rem                  NMake presets that go through ..\bin\*_with_setcc.bat (so no
rem                  developer prompt is needed).
rem      x64|x86  -> architecture (default x64). x86 is NMake presets only.
rem
rem    e.g.  vctest.bat std vc145           baseline on VC 14.5
rem          vctest.bat std vc145 c++17     baseline, restricted to the C++17 surface
rem          vctest.bat vc145               the ccwrap build
rem          vctest.bat vc120 x86           an old vc (NMake preset)
rem
rem  To build a subset, call cmake directly:  cmake --preset vc145 -DTST_ONLY=locale
setlocal
cd /d "%~dp0"

set "TGT=2023"
set "CXXSTD=c++latest"
set "VCVER=vc145"
set "ARCH=x64"

:ARGS
if "%~1"=="" goto ARGS_DONE
if /i "%~1"=="std"   set "TGT="
if /i "%~1"=="x64"   set "ARCH=x64"
if /i "%~1"=="x86"   set "ARCH=x86"
if /i "%~1"=="win32" set "ARCH=x86"
echo %~1| findstr /i /b /c:"c++" >nul && set "CXXSTD=%~1"
echo %~1| findstr /i /b /r /c:"vc[0-9]" >nul && set "VCVER=%~1"
shift
goto ARGS
:ARGS_DONE

set "PRESET=%VCVER%-%ARCH%"
if /i "%ARCH%"=="x64" (
  if /i "%VCVER%"=="vc145" set "PRESET=vc145"
  if /i "%VCVER%"=="vc143" set "PRESET=vc143"
)

echo # preset=%PRESET%  TEST_TARGET_CXX=%TGT%  TST_MSVC_STD=%CXXSTD%
rem  TEST_TARGET_CXX empty = no ccwrap at all (the tag then has no -tgt suffix).
cmake --preset %PRESET% -DTEST_TARGET_CXX=%TGT% -DTST_MSVC_STD=%CXXSTD%
if errorlevel 1 ( echo CONFIGURE FAILED & exit /b 1 )
cmake --build --preset %PRESET%
if errorlevel 1 ( echo BUILD FAILED & exit /b 1 )
ctest --preset %PRESET%
exit /b %ERRORLEVEL%
