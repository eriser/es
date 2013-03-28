call doit.bat

if Errorlevel 1 goto error
cd Release
strip --strip-all -v -R .comment %NAME%.exe
if exist %NAME%_upx.exe del %NAME%_upx.exe
..\..\bin\util\upx.exe -9 -o %NAME%_upx.exe %NAME%.exe
dir %NAME%.exe
cd ..
rem %NAME%_upx.exe

:end
rem pause
goto :EOF

:error
pause
