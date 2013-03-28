call doit.bat
if Errorlevel 1 goto end
cd Release
strip --strip-all -v -R .comment %NAME%.exe
if exist %NAME%_upx.exe del %NAME%_upx.exe
..\..\bin\util\upx.exe -9 -o %NAME%_upx.exe %NAME%.exe
dir %NAME%.exe
rem %NAME%_upx.exe
goto :EOF

:error
pause
exit 1
