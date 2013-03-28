strip --strip-all -v -R .comment %1.exe
if exist %1_upx.exe del %1_upx.exe
..\..\bin\util\upx.exe -9 -o %1_upx.exe %1.exe
dir %1.exe
