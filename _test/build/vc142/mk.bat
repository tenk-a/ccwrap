@echo off
setlocal

set SRCDIR=..\..\src
set INCDIR=..\..\..\..\ccwrap\vc
set INCDIRXX=..\..\..\..\ccwrap\cxx
set CCOPTS=-EHsc -std:c++17 -Zc:__cplusplus

if exist smp.exe del smp.exe
if exist smp_c.exe del smp_c.exe
if exist smp_cxx.exe del smp_cxx.exe
if exist smp_stl.exe del smp_stl.exe
if exist smp_math.exe del smp_math.exe

@echo [c++ core]
cl %CCOPTS% -I%INCDIR% -I%SRCDIR% %SRCDIR%\smp.cpp
@echo [c core]
cl -I%INCDIR% -I%SRCDIR% %SRCDIR%\smp_c.c
@echo [c++]
cl %CCOPTS% -I%INCDIR% -I%SRCDIR% %SRCDIR%\smp_cxx.cpp
@echo [c++ stl]
cl %CCOPTS% -I%INCDIR% -I%SRCDIR% %SRCDIR%\smp_stl.cpp
@echo [math]
cl %CCOPTS% -I%INCDIR% -I%SRCDIR% %SRCDIR%\smp_math.cpp

@echo [Compiling for error]
cl %CCOPTS% -DCOMPILING_FOR_ERROR -I%INCDIR% -I%SRCDIR% %SRCDIR%\smp.cpp

if exist smp.exe smp.exe
if exist smp_c.exe smp_c.exe
if exist smp_cxx.exe smp_cxx.exe
if exist smp_stl.exe smp_stl.exe
if exist smp_math.exe smp_math.exe

endlocal
