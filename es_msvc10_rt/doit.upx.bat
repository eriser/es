call doit.bat
if Errorlevel 1 goto error

cd Release_Win32
call ..\..\bin\util\compress_copy.bat %NAME%
cd ..
rem cd Release_x64
rem call ..\..\bin\util\compress_copy.bat %NAME%
rem cd ..

:end
rem pause
goto :EOF

:error
pause
