@echo off
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
cmake --preset %PRESET% -DTEST_TARGET_CXX=%TGT% -DTST_MSVC_STD=%CXXSTD%
if errorlevel 1 ( echo CONFIGURE FAILED & exit /b 1 )
cmake --build --preset %PRESET%
if errorlevel 1 ( echo BUILD FAILED & exit /b 1 )
ctest --preset %PRESET%
exit /b %ERRORLEVEL%
