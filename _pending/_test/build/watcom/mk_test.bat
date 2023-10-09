rem ‚Ü‚¾ƒRƒ“ƒpƒCƒ‹‚ª’Ê‚ç‚È‚¢.
@echo off
setlocal
set CCWRAP_ROOT=..\..\..
set TestName=test
set SRCDIR=..\..\src\%TestName%
set INCDIR=..\..\..\watcom
set INCDIR2=%CCWRAP_ROOT%\ccwrap

set CCXX=wcl386
set CCOPTS_REL=-O2 -DNDEBUG
set CCOPTS_DBG=-D_DEBUG
set CCWARN=%CCWARN%
set CCOPTS=%CCOPTS% -cc++ -xs -xr -bcl=nt  -D_CONSOLE -DCCWRAP_ASSERT_NO_ABORT -DUSE_TEST -DCCWRAP_TEST_LOG_LITE
set FIRSTINC=-fi=ccwrap_header.h
set LOPTS=%LOPTS% -EHac -GF
if "%TestName%"=="" set TestName=test
set INCS=%INCS% -I%INCDIR% -I%INCDIR2% -I%SRCDIR%
set LIBS=%LIBS% kernel32.lib user32.lib shell32.lib shlwapi.lib
set SRCS=%SRCDIR%\*.cpp

if exist *.exe del *.exe

set OBJDIR=
set ABXOPTS=
set Conf=
if /I "%Conf%"=="release" (
  set Conf=release
  set CCOPTS=%CCOPTS% %CCOPTS_REL%
  set LOPTS=%LOPTS% %CCOPTS_REL%
  set ABXOPTS=%ABXOPTS% -xm
  if exist test.pdb del test.pdb
  if exist test.ilk del test.ilk
) else (
  set Conf=debug
  set CCOPTS=%CCOPTS% %CCOPTS_DBG%
  set LOPTS=%LOPTS% %CCOPTS_DBG%
  set ABXOPTS=%ABXOPTS%
)
set EXENAME=%TestName%_%VcVer%_%Arch%_%Conf%.exe
set OBJDIR=obj\%TestName%\%VcVer%_%Arch%_%Conf%
mkdir %OBJDIR%

set CCOPTS=%CCOPTS% %CCWARN% %FIRSTINC% -I. %INCS%

abx %ABXOPTS% -s -x -cu%OBJDIR%\@x.obj %SRCS% =%CCXX% -c %CCOPTS% -fo=%OBJDIR%\@x.obj @Rbf
del %OBJDIR%\set_objs.bat
set OBJS=
abx -s -k$ %OBJDIR%\*.obj "=@set OBJS=%%OBJS%% %OBJDIR%\$c" >>%OBJDIR%\set_objs.bat
call %OBJDIR%\set_objs.bat

@echo [CMD] %CCXX% -fe=%EXENAME% %LOPTS% %OBJS% %LIBS% %LAST_OPTS%
if exist %EXENAME% del %EXENAME%
%CCXX% -fe=%EXENAME% %LOPTS% %OBJS% %LIBS% %LAST_OPTS%

if exist %EXENAME% %EXENAME%
endlocal
