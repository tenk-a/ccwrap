@echo off
setlocal
call "%~dp0..\..\bin\setcc.bat" "%CCWRAP_VC_VERSION%" "%CCWRAP_VC_ARCH%" >nul
if errorlevel 1 exit /b %ERRORLEVEL%
lib.exe %*
exit /b %ERRORLEVEL%
