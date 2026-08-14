@echo off
setlocal
call "%~dp0..\..\bin\setcc.bat" "%CCWRAP_VC_VERSION%" "%CCWRAP_VC_ARCH%" >nul
if errorlevel 1 exit /b %ERRORLEVEL%
where.exe rc.exe >nul 2>nul
if not errorlevel 1 (
    rc.exe %*
    exit /b
)

set "CCWRAP_KIT_HOST=x86"
if /I "%PROCESSOR_ARCHITECTURE%"=="AMD64" set "CCWRAP_KIT_HOST=x64"
if /I "%PROCESSOR_ARCHITEW6432%"=="AMD64" set "CCWRAP_KIT_HOST=x64"
if /I "%CCWRAP_VC_ARCH%"=="x64" set "CCWRAP_KIT_HOST=x64"
if /I "%CCWRAP_VC_ARCH%"=="arm64" set "CCWRAP_KIT_HOST=x64"
if /I "%CCWRAP_VC_ARCH%"=="ARM64" set "CCWRAP_KIT_HOST=x64"

for %%D in (
    "C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\%CCWRAP_KIT_HOST%"
    "C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0\%CCWRAP_KIT_HOST%"
    "C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\%CCWRAP_KIT_HOST%"
    "C:\Program Files (x86)\Windows Kits\8.1\bin\%CCWRAP_KIT_HOST%"
    "C:\Program Files (x86)\Windows Kits\8.0\bin\%CCWRAP_KIT_HOST%"
) do (
    if exist "%%~D\rc.exe" (
        "%%~D\rc.exe" %*
        exit /b
    )
)

echo rc.exe not found.
exit /b 9009
