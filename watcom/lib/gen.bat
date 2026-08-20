@echo off
setlocal EnableExtensions
rem ===========================================================================

cd /d "%~dp0"

set "WHAT=%~1"
if "%WHAT%"=="" set "WHAT=all"
if /i "%WHAT%"=="std"     ( set "WHAT=all" & set "SET1=std" )
if /i "%WHAT%"=="default" ( set "WHAT=all" & set "SET1=default" )
if /i "%WHAT%"=="xrxs"    ( set "WHAT=all" & set "SET1=xrxs" )
if /i "%WHAT%"=="noeh"    ( set "WHAT=all" & set "SET1=noeh" )
if /i "%WHAT%"=="full"    ( set "WHAT=all" & set "SET1=full" )
set "OPTS=%~2"
if "%OPTS%"=="" set "OPTS=%SET1%"
if "%OPTS%"=="" set "OPTS=std"

if /i "%WHAT%"=="clean" goto L_CLEAN

set "SFX="
if /i "%OPTS%"=="std"     set "SFX=."
if /i "%OPTS%"=="all"     set "SFX=."
if /i "%OPTS%"=="default" set "SFX=-default"
if /i "%OPTS%"=="xrxs"    set "SFX=-xrxs"
if /i "%OPTS%"=="noeh"    set "SFX=-noeh"
if /i "%OPTS%"=="full"    set "SFX=-full"
if "%SFX%"=="" (
    echo gen: unknown set "%OPTS%"
    echo usage: gen [all^|win32^|dos32^|release^|debug^|clean] [std^|default^|xrxs^|noeh^|full]
    exit /b 2
)
if "%SFX%"=="." set "SFX="

set "PRESETS="
if /i "%WHAT%"=="all"     set "PRESETS=watcom watcom-debug watcom-dos32 watcom-dos32-debug"
if /i "%WHAT%"=="win32"   set "PRESETS=watcom watcom-debug"
if /i "%WHAT%"=="dos32"   set "PRESETS=watcom-dos32 watcom-dos32-debug"
if /i "%WHAT%"=="release" set "PRESETS=watcom watcom-dos32"
if /i "%WHAT%"=="debug"   set "PRESETS=watcom-debug watcom-dos32-debug"

if "%PRESETS%"=="" (
    echo gen: unknown argument "%WHAT%"
    echo usage: gen [all^|win32^|dos32^|release^|debug^|clean] [std^|default^|xrxs^|noeh^|full]
    exit /b 2
)

set "FAILED="
set "BUILT="
for %%P in (%PRESETS%) do call :L_ONE %%P

echo.
if not "%FAILED%"=="" (
    echo [gen] FAILED:%FAILED%
    exit /b 1
)
echo [gen] built:%BUILT%
echo [gen] libraries now present in watcom/lib (this run rebuilt only the above):
dir /b nt\ccw*.lib dos\ccw*.lib 2>nul
if exist debug (
    echo [gen]   debug/
    dir /b debug\nt\ccw*.lib debug\dos\ccw*.lib 2>nul
)
exit /b 0

rem ---------------------------------------------------------------------------
:L_ONE
echo.
echo === %1%SFX% ===
set "CFG=%1"
if /i "%OPTS%"=="full" set "CFG=%1-full"
if /i "%OPTS%"=="xrxs" set "CFG=%1-xrxs"
cmake --preset %CFG%
if errorlevel 1 goto L_ONE_FAIL
cmake --build --preset %1%SFX%
if errorlevel 1 goto L_ONE_FAIL
set "BUILT=%BUILT% %1%SFX%"
goto :eof
:L_ONE_FAIL
set "FAILED=%FAILED% %1%SFX%"
goto :eof

rem ---------------------------------------------------------------------------
:L_CLEAN
if exist _build (
    echo [gen] removing _build
    rmdir /s /q _build
)
exit /b 0
