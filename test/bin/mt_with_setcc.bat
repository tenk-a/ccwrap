@echo off
setlocal
call "%~dp0..\..\bin\setcc.bat" "%CCWRAP_VC_VERSION%" "%CCWRAP_VC_ARCH%" >nul
if errorlevel 1 exit /b %ERRORLEVEL%

rem Prefer the newest Windows 10 SDK mt.exe over whatever the toolchain puts on PATH.
rem An old mt.exe SILENTLY DROPS manifest elements it does not know: VS2013's warns
rem "81010002 Unrecognized Element activeCodePage" and then writes a manifest without
rem it, so test/src/utf8_acp.manifest has no effect at all. Measured on vc120: with the
rem 10.0.26100 mt.exe the element IS embedded. A newer mt only ever understands more,
rem so preferring it is safe for the old toolchains.

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
    if exist "%%~D\mt.exe" (
        "%%~D\mt.exe" %*
        exit /b
    )
)

rem Nothing newer installed: fall back to whatever setcc left on PATH.
where.exe mt.exe >nul 2>nul
if not errorlevel 1 (
    mt.exe %*
    exit /b
)

echo mt.exe not found.
exit /b 9009
