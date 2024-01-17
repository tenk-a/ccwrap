pushd _cxx_lang_test

call ..\..\setcc.bat bcc55
@echo on
pushd ..\_test\bc55
call cxx_lang_test.bat
popd

call ..\..\setcc.bat dmc
@echo on
pushd ..\_test\dmc
call cxx_lang_test.bat
popd

call ..\..\setcc.bat ow
@echo on
pushd ..\_test\ow
call cxx_lang_test.bat
popd

call ..\..\setcc.bat msys2
@echo on
pushd ..\..\_test\build\mingw
call cxx_lang_test.bat
popd

call ..\..\setcc.bat vc120
@echo on
pushd ..\..\_test\build\vc
call cxx_lang_test.bat
popd

popd
