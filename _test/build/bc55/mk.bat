@echo off
setlocal
set SRCDIR=..\..\src
set INCDIR=..\..\..\..\ccwrap\borland
set CXXDIR=..\..\..\..\ccwrap\cxx
set INCDIR2=.
rem set INCDIR2=..\..\..\..\ccwrap\boost2std
rem set BOOSTDIR=d:\libs_vc\boost_1_65_0
rem bcc32 -I%SRCDIR% -I%INCDIR% %SRCDIR%\smp.cpp
rem bcc32 -I%SRCDIR% -I%INCDIR% -I%INCDIR2% %SRCDIR%\smp.cpp

if exist smp.exe del smp.exe
if exist smp_c.exe del smp_c.exe
if exist smp_cxx.exe del smp_cxx.exe

bcc32 -I%SRCDIR% -I%INCDIR% -I%CXXDIR% %SRCDIR%\smp.cpp
bcc32 -I%SRCDIR% -I%INCDIR% %SRCDIR%\smp_c.c
bcc32 -I%SRCDIR% -I%INCDIR% -I%CXXDIR% %SRCDIR%\smp_cxx.cpp

if exist smp.exe smp.exe
if exist smp_c.exe smp_c.exe
if exist smp_cxx.exe smp_cxx.exe

echo -
echo [COMPILE ERROR CHECK]
bcc32 -DCOMPILING_FOR_ERROR -I%SRCDIR% -I%INCDIR% -I%CXXDIR% %SRCDIR%\smp.cpp

endlocal
