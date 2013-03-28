call doit.bat "SKIP_x64"
if Errorlevel 1 goto error

cd Release_Win32
call ..\..\bin\util\compress_copy.bat %NAME%
cd ..

:end
rem pause
goto :EOF

:error
pause
