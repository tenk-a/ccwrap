@echo off
setlocal

set SRCDIR=..\..\src\smp
set INCDIR=..\..\..\gcc
set INCDIR2=..\..\..\ccwrap
set CCOPTS= -I%INCDIR% -I%INCDIR2% -Wno-unused-local-typedefs -include ccwrap_header.h

set CC=gcc
set CXXC=g++
set WRN_OPT=-Wall -Wno-comment

if exist *.exe del *.exe

@echo [c++03 core]
%CXXC% -o smp.exe -std=c++03 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp

@echo [c++11 core]
%CXXC% -o smp11.exe -std=c++11 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp

@echo [c++14 core]
%CXXC% -o smp14.exe -std=c++14 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp

@echo [c++17 core]
%CXXC% -o smp17.exe -std=c++17 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp

@echo [c++20 core]
%CXXC% -o smp20.exe -std=c++20 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp.cpp

rem @echo [c90 core]
rem %CC%   -o smp_c90.exe -std=c90  -Wno-c++20-compat %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp_c.c

@echo [c99 core]
%CC%   -o smp_c99.exe -std=c99 %WRN_OPT% %CCOPTS% -I%SRCDIR% %SRCDIR%\smp_c.c

@echo -
@echo === [Compiling for error] Begin ===============================================
%CXXC% -o smp.exe -std=c++11 %WRN_OPT% %CCOPTS% -I%SRCDIR% -DCOMPILING_FOR_ERROR %SRCDIR%\smp.cpp
@echo === [Compiling for error] End =================================================

if exist smp03.exe   smp03.exe
if exist smp11.exe   smp11.exe
if exist smp14.exe   smp14.exe
if exist smp17.exe   smp17.exe
if exist smp20.exe   smp20.exe
if exist smp_c90.exe smp_c90.exe
if exist smp_c99.exe smp_c99.exe
if exist *obj        del *.obj

endlocal
