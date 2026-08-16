@echo off
setlocal EnableExtensions
rem ===========================================================================
rem  gen.bat -- build every prebuilt libccwcxx.lib variant in one go.
rem
rem    gen [WHAT] [OPTS]
rem
rem    WHAT   all       all four configurations (default)
rem           win32     Win32 release + debug
rem           dos32     32-bit DOS release + debug
rem           release   Win32 + DOS, release only
rem           debug     Win32 + DOS, debug only
rem           clean     remove the CMake build trees (_build/), keep the .lib output
rem
rem    SET    std       (default) <os>-std and <os>-std-noeh
rem           default   just the unsuffixed one (-xst -xr: the recommended build)
rem           xrxs      just the -xrxs one (-xs -xr: test only, see below)
rem           noeh      just the -noeh one (neither -xs nor -xr)
rem           full      ccwstd / -fs-char / -xst too (still no -xrxs)
rem
rem  **Nothing builds -xrxs except `gen xrxs`, not even `full`.** -xs miscompiles at
rem  -o<n> (A1''), so it must not be used for real programs; it exists only so the
rem  suites can measure that. The target itself is opt-in (CCW_LIB_XS).
rem
rem  The default build is deliberately small. ccwstd, -fs-char and -xst are only
rem  produced by `full` (they are opt-in cache variables: CCW_LIB_CCWSTD /
rem  CCW_LIB_FS_CHAR / CCW_LIB_XST). **test/ needs -xst and ccwstd, so use
rem  `gen full` before running the suites.**
rem
rem    gen win32 default   -> win32-std, release and debug
rem    gen release noeh    -> win32-std-noeh and dos32-std-noeh, release only
rem    gen full            -> all four configurations, every variant
rem
rem  A single variant is just a target, and **the target name is the folder name**
rem  with '-' turned into '_':
rem    cmake --build --preset watcom --target ccwrap_win32_std_fs_char_xst
rem
rem  One CMake configuration builds EVERY variant of that target+build type, so
rem  the four configurations below cover the whole matrix:
rem
rem    watcom             -> watcom/lib/win32-<ns>[-fs-char][-<opts>]/
rem    watcom-debug       -> watcom/lib/debug/win32-...
rem    watcom-dos32       -> watcom/lib/dos32-std[-<opts>]/
rem    watcom-dos32-debug -> watcom/lib/debug/dos32-std[-<opts>]/
rem
rem  <ns>   = std | ccwstd            (ccwstd is Win32 only)
rem  -lfn   = DOS long file names (-D__WATCOM_LFN__). **DOS only, and always built**:
rem           it is an ABI switch (_MAX_PATH 144->260, NAME_MAX 12->259 resize
rem           struct _finddata_t), so a program built with -D__WATCOM_LFN__ must link
rem           the -lfn library. Mixing links cleanly and then overruns a buffer.
rem  <opts> = what was changed from the recommended default (-xst -xr): nothing,
rem           -xrxs (-xs -xr), -xst (-xst only), -xr (-xr only), -noeh (neither).
rem           -xs / -xss miscompile at -o<n> (ccwrap_watcom_bugs.md A1''), so the
rem           official build is -xst and the -xs one is kept only for testing.
rem  A program must link the variant that matches how IT was built -- mixing
rem  links cleanly and then misbehaves.
rem ===========================================================================

cd /d "%~dp0"

set "WHAT=%~1"
if "%WHAT%"=="" set "WHAT=all"
rem  `gen full` -- a set given where a configuration was expected means "all of them"
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
echo [gen] libraries now present under watcom/lib (this run rebuilt only the above):
dir /b /ad win32-* dos32-* 2>nul
if exist debug (
    echo [gen]   debug/
    dir /b /ad debug\win32-* debug\dos32-* 2>nul
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
