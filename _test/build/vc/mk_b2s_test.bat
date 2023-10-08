setlocal
rem set BoostDir=d:\libs_vc\boost_1_68_0
set BOOSTDIR=d:\libs_vc\boost_1_72_0

call mk.bat boost2std_test boost:%BoostDir% %*

call mk.bat test boost:%BoostDir% %*
endlocal
