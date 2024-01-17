abx -s %SRCDIR%\*.cpp -otmp_test.bat =echo @c : @n%CC% %OPTS% @f %LIBS%@nif exist @x.exe @x.exe@necho errorlevel=%%errorlevel%%@necho:@necho:@n

if exist cxx_lang_test.log del cxx_lang_test.log
call tmp_test.bat >cxx_lang_test.log
rem del tmp_test.bat
goto :SUB_END

:SUB_END
