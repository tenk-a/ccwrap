:: open watcom hello sample
setlocal
pushd %~dp0
pushd ..\..
set CCW_ROOT=%CD%
popd

if not exist bin mkdir bin

set O_INC=-i=%CCW_ROOT%/watcom/std
set O_FI=-fi=%CCW_ROOT%/watcom/std/ccwrap_header.h
set O_WIN=-bt=nt -l=nt -bm
set O_DOS4G=-bt=dos -l=dos4g
set O_EH=-xr -xst
set O_NE=-xd
set O_REL=-DNDEBUG
set O_DBG=-d2
set "O_LNKREL=option eliminate option vfremoval"
set "O_LIBWIN=%CCW_ROOT%\watcom\lib\nt"
set "O_LIBDOS=%CCW_ROOT%\watcom\lib\dos"

wcl386 %O_DBG% %O_WIN% %O_EH% %O_INC% %O_FI% -Fe=bin\ow-w32-eh-dbg.exe hello.cpp  %O_LIBWIN%/ccw-eh3r.lib
wcl386 %O_REL% %O_WIN% %O_EH% %O_INC% -Fe=bin\ow-w32-eh.exe hello.cpp  -"LIBPATH %O_LIBWIN% %O_LNKREL%"
wcl386 %O_REL% %O_WIN% %O_NE% %O_INC% %O_FI% -Fe=bin\ow-w32-ne.exe hello.cpp  -"LIBPATH %O_LIBWIN% %O_LNKREL%"

wcl386 %O_REL% %O_DOS4G% %O_NE% %O_INC% %O_FI% -Fe=bin\ow-d32-ne.exe hello.cpp -"LIBPATH %O_LIBDOS% %O_LNKREL%"
wcl386 -D__WATCOM_LFN__ %O_REL% %O_DOS4G% %O_EH% %O_INC% -Fe=bin\ow-d32lfn-eh.exe hello.cpp -"LIBPATH %O_LIBDOS% %O_LNKREL%"

set "INCLUDE=%CCW_ROOT%/watcom/std;%INCLUDE%"
set "LIB=%CCW_ROOT%/watcom/lib/dos;%LIB%"
wcl386 %O_REL% %O_DOS4G% %O_EH% -fe=bin\ow-d32-eh.exe hello.cpp -"%O_LNKREL%"

del *.obj *.err
popd
endlocal
