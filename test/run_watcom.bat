@echo off
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

:cmake_run
set "EXTRA=%~1"
cd /d "%HERE%"
cmake --preset watcom %EXTRA%
if errorlevel 1 ( echo CONFIGURE FAILED & exit /b 1 )
cmake --build --preset watcom
if errorlevel 1 ( echo BUILD FAILED & exit /b 1 )
ctest --preset watcom
exit /b %ERRORLEVEL%

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
set "OUT=%HERE%_build\dos"
if "%DOSBOX%"=="" set "DOSBOX=%HERE%..\my_emu\DOSBox-X\dosbox-x.exe"
set "FRE=-fr=%OUT%\ccw.err"
if not exist "%OUT%" mkdir "%OUT%"
cd /d "%HERE%.."
exit /b 0
