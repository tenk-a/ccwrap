@echo off
setlocal

set SRCDIR=..\..\src\smp
set CCOPTS=-EHsc -FIccwrap_header.h

set VcVer=
@if /I not "%PATH:Microsoft Visual Studio .NET 2003=%"=="%PATH%" set VcVer=vc71
@if /I not "%PATH:Microsoft Visual Studio 8=%"=="%PATH%"    set VcVer=vc80
@if /I not "%PATH:Microsoft Visual Studio 9.0=%"=="%PATH%"  set VcVer=vc90
@if /I not "%PATH:Microsoft Visual Studio 10.0=%"=="%PATH%" set VcVer=vc100
@if /I not "%PATH:Microsoft Visual Studio 11.0=%"=="%PATH%" set VcVer=vc110
@if /I not "%PATH:Microsoft Visual Studio 12.0=%"=="%PATH%" set VcVer=vc120
@if /I not "%PATH:Microsoft Visual Studio 13.0=%"=="%PATH%" set VcVer=vc130
@if /I not "%PATH:Microsoft Visual Studio 14.0=%"=="%PATH%" set VcVer=vc140
@if /I not "%PATH:Microsoft Visual Studio\2017=%"=="%PATH%" set VcVer=vc141
@if /I not "%PATH:Microsoft Visual Studio\2019=%"=="%PATH%" set VcVer=vc142
if "%VcVer%"=="" goto ERR
set Arch=Win32
@if "%VcVer%"=="vc142"    if /I not "%PATH:\bin\HostX64\x64=%"=="%PATH%" set Arch=x64
@if "%VcVer%"=="vc141"    if /I not "%PATH:\bin\HostX64\x64=%"=="%PATH%" set Arch=x64
@if /I not "%PATH:Microsoft Visual Studio 14.0\VC\BIN\amd64=%"=="%PATH%" set Arch=x64
@if /I not "%PATH:Microsoft Visual Studio 13.0\VC\BIN\amd64=%"=="%PATH%" set Arch=x64
@if /I not "%PATH:Microsoft Visual Studio 12.0\VC\BIN\amd64=%"=="%PATH%" set Arch=x64
@if /I not "%PATH:Microsoft Visual Studio 11.0\VC\BIN\amd64=%"=="%PATH%" set Arch=x64
@if /I not "%PATH:Microsoft Visual Studio 10.0\VC\BIN\amd64=%"=="%PATH%" set Arch=x64
@if /I not "%PATH:Microsoft Visual Studio 9.0\VC\BIN\amd64=%"=="%PATH%"  set Arch=x64
@if /I not "%PATH:Microsoft Visual Studio 8\VC\BIN\amd64=%"=="%PATH%"    set Arch=x64

set Vc2015orLater= 
if "%VcVer%"=="vc140" set Vc2015orLater=1
if "%VcVer%"=="vc141" set Vc2015orLater=1
if "%VcVer%"=="vc142" set Vc2015orLater=1

set INCDIR=..\..\..\vc2013_or_earlier
if "%Vc2015orLater%"=="1" set INCDIR=..\..\..\vc2015_or_later


if exist smp.exe del smp.exe
if exist smp_c.exe del smp_c.exe

@echo [c++ core]
cl %CCOPTS% -I%INCDIR% -I%SRCDIR% %SRCDIR%\smp.cpp
@echo [c core]
cl -I%INCDIR% -I%SRCDIR% %SRCDIR%\smp_c.c

@echo -
@echo === [Compiling for error] Begin ===============================================
cl %CCOPTS% -DCOMPILING_FOR_ERROR -I%INCDIR% -I%SRCDIR% %SRCDIR%\smp.cpp
@echo === [Compiling for error] End =================================================

if exist smp.exe smp.exe
if exist smp_c.exe smp_c.exe
del *.obj

endlocal
