@echo off
rem  Thin wrapper for the Open Watcom test run. The build itself lives in CMake
rem  (test\CMakeLists.txt + each suite's CMakeLists); this only picks the suites and
rem  calls configure -> build -> test.
rem
rem  NOTE: keep this file ASCII-only. cmd.exe reads .bat in the OEM code page and `goto`
rem  seeks by BYTE offset, so UTF-8 Japanese in comments derails label lookup.
rem
rem    Usage:  run_watcom.bat [target] [watcom_dir]
rem      target     : htest (alias cpp) | win | cfail | all | dos4g | dos16
rem      watcom_dir : only meaningful for dos4g/dos16 (%WATCOM%, default C:\WATCOM).
rem                   The CMake targets use the preset's WATCOM (C:/WATCOM).
rem
rem    e.g.  run_watcom.bat htest    per-header C++ suite + the compile-fail tests
rem          run_watcom.bat win      the non-per-header C smoke tests (etc_test\watcom)
rem          run_watcom.bat all      every suite (same as test\run.bat watcom)
rem          run_watcom.bat dos16    16-bit DOS build only
rem
rem  Only the DOS targets are still driven by hand: CMake has no DOS Open Watcom
rem  toolchain support. Build only -- running 16/32-bit DOS binaries needs a DOS box.
setlocal
set "TGT=%~1"
if "%TGT%"=="" set "TGT=htest"
set "HERE=%~dp0"

if /i "%TGT%"=="all"   goto cm_all
if /i "%TGT%"=="htest" goto cm_htest
if /i "%TGT%"=="cpp"   goto cm_htest
if /i "%TGT%"=="win"   goto cm_win
if /i "%TGT%"=="cfail" goto cm_cfail
if /i "%TGT%"=="dos4g" goto dos4g
if /i "%TGT%"=="dos16" goto dos16
echo Unknown target: %TGT%  (use htest^|cpp^|win^|cfail^|all^|dos4g^|dos16)
exit /b 2

:cm_all
rem  Pass TST_SUITES explicitly: it is a cache variable, so a previous `cfail`/`win` run
rem  in the same build dir would otherwise stay in effect and silently narrow the run.
call :cmake_run "-DTST_SUITES=auto"
exit /b %ERRORLEVEL%

:cm_htest
call :cmake_run "-DTST_SUITES=cxx_test;compile_fail"
exit /b %ERRORLEVEL%

:cm_win
call :cmake_run "-DTST_SUITES=c_test;etc_watcom"
exit /b %ERRORLEVEL%

:cm_cfail
call :cmake_run "-DTST_SUITES=compile_fail"
exit /b %ERRORLEVEL%

rem  :cmake_run <extra configure args>  -- configure -> build -> test
:cmake_run
set "EXTRA=%~1"
cd /d "%HERE%"
cmake --preset watcom %EXTRA%
if errorlevel 1 ( echo CONFIGURE FAILED & exit /b 1 )
cmake --build --preset watcom
if errorlevel 1 ( echo BUILD FAILED & exit /b 1 )
ctest --preset watcom
exit /b %ERRORLEVEL%

rem  DOS targets (not on CMake). Built by hand, then run under DOSBox-X when it is
rem  present: the emulator returns 0 to the host whatever the guest did, so the guest
rem  writes its stdout to a file on the mounted drive and we grep that.
rem  Pass a 3rd argument of "build" to stop after the build.
:dos4g
call :dos_setup "%~2"
echo # Open Watcom (%WATCOM%)  target=dos4g (32-bit DOS)
wcl386 -q -bt=dos -l=dos4g %FRE% -fi=watcom\std\ccwrap_header.h -i=watcom/std -fo="%OUT%\wct4.obj" -fe="%OUT%\wct4.exe" test\etc_test\watcom\watcom_ctest.c
if errorlevel 1 ( echo BUILD FAILED & exit /b 1 )
call :dos_exec wct4.exe "%~3"
exit /b %ERRORLEVEL%

:dos16
call :dos_setup "%~2"
echo # Open Watcom (%WATCOM%)  target=dos16 (16-bit DOS, large model)
wcl -q -bt=dos -ml -l=dos %FRE% -fi=watcom\std\ccwrap_header.h -i=watcom/std -fo="%OUT%\wct16.obj" -fe="%OUT%\wct16.exe" test\etc_test\watcom\watcom_ctest.c
if errorlevel 1 ( echo BUILD FAILED & exit /b 1 )
call :dos_exec wct16.exe "%~3"
exit /b %ERRORLEVEL%

rem  :dos_exec <exe> <"build" to skip running>
:dos_exec
if /i "%~2"=="build" ( echo BUILD_OK  ^(run skipped by request^) & exit /b 0 )
if not exist "%DOSBOX%" ( echo BUILD_OK  ^(DOSBox-X not at %DOSBOX%; run skipped^) & exit /b 0 )
powershell -NoProfile -ExecutionPolicy Bypass -File "%HERE%scripts\dosbox_run.ps1" -WorkDir "%OUT%" -Exe %~1
if errorlevel 1 ( echo RUN FAILED & exit /b 1 )
findstr /c:"all checks passed" "%OUT%\dosout.txt" >nul
if errorlevel 1 ( echo RUN FAILED  ^(see %OUT%\dosout.txt^) & exit /b 1 )
echo RUN_OK
exit /b 0

:dos_setup
if not "%~1"=="" set "WATCOM=%~1"
if "%WATCOM%"=="" set "WATCOM=C:\WATCOM"
set "PATH=%WATCOM%\binnt64;%WATCOM%\binnt;%PATH%"
set "EDPATH=%WATCOM%\eddat"
set "INCLUDE=%WATCOM%\h"
rem  Keep every artefact inside the repo (test/_build is git-ignored).
set "OUT=%HERE%_build\dos"
if "%DOSBOX%"=="" set "DOSBOX=%HERE%..\my_emu\DOSBox-X\dosbox-x.exe"
rem  Without -fr=, wcl386 drops <source>.err into the current dir (the repo root here).
set "FRE=-fr=%OUT%\ccw.err"
if not exist "%OUT%" mkdir "%OUT%"
cd /d "%HERE%.."
exit /b 0
