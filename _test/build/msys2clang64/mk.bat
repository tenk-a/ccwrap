@echo off
setlocal
set SRCDIR=..\..\src
set INCDIR=
set INCDIR=..\..\..\..\ccwrap\cxx03clang
set CXXDIR=..\..\..\..\ccwrap\cxx
set COPTS=

if exist smp.exe del smp.exe
if exist smp_c.exe del smp_c.exe
if exist smp_cxx*.exe del smp_cxx*.exe
if exist smp_stl*.exe del smp_stl*.exe

clang++ -o smp.exe -std=c++11 -Wall %COPTS% -I%SRCDIR% %SRCDIR%\smp.cpp
clang   -o smp_c.exe -std=c11 -Wall %COPTS% -I%SRCDIR% %SRCDIR%\smp_c.c

clang++ -o smp_cxx03.exe -std=c++03 -Wall %COPTS% -I%INCDIR% -I%CXXDIR% -I%SRCDIR% %SRCDIR%\smp_cxx.cpp
rem clang++ -o smp_cxx11.exe -std=c++11 -Wall %COPTS% -I%SRCDIR% %SRCDIR%\smp_cxx.cpp
clang++ -o smp_cxx17.exe -std=c++17 -Wall %COPTS% -I%SRCDIR% %SRCDIR%\smp_cxx.cpp
clang++ -o smp_stl14.exe -std=c++14 %COPTS%  -I%INCDIR% -I%CXXDIR% -I%SRCDIR% %SRCDIR%\smp_stl.cpp
clang++ -o smp_stl17.exe -std=c++17 %COPTS% -I%SRCDIR% %SRCDIR%\smp_stl.cpp

clang++ -o smp.exe -std=c++11 -Wall %COPTS% -I%SRCDIR% -DCOMPILING_FOR_ERROR %SRCDIR%\smp.cpp

if exist smp.exe smp.exe
if exist smp_c.exe smp_c.exe
if exist smp_cxx03.exe smp_cxx03.exe
rem if exist smp_cxx11.exe smp_cxx11.exe
if exist smp_cxx17.exe smp_cxx17.exe
if exist smp_stl14.exe smp_stl14.exe
if exist smp_stl17.exe smp_stl17.exe

endlocal
