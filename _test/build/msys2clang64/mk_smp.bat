@echo off
setlocal

set SRCDIR=..\..\src\smp
set CCOPTS= -Wno-unused-local-typedefs -include ..\ccwrap\ccwrap_std.h

rem set INCDIR=..\..\..\clang_cxx03

if exist smp.exe del smp.exe
if exist smp_c.exe del smp_c.exe

@echo [c++ core]
clang++ -o smp.exe -std=c++11 -Wall %COPTS% -I%SRCDIR% %SRCDIR%\smp.cpp
@echo [c core]
clang   -o smp_c.exe -std=c11 -Wall %COPTS% -I%SRCDIR% %SRCDIR%\smp_c.c

@echo -
@echo === [Compiling for error] Begin ===============================================
clang++ -o smp.exe -std=c++11 -Wall %COPTS% -I%SRCDIR% -DCOMPILING_FOR_ERROR %SRCDIR%\smp.cpp
@echo === [Compiling for error] End =================================================

if exist smp.exe smp.exe
if exist smp_c.exe smp_c.exe
del *.obj

endlocal
