set CFG=Release
mingw32-make
rem if Errorlevel 1 goto error
cd Release
if exist libes.a del libes.a
ar rs libes.a *.o
copy libes.a ..
cd ..
goto :EOF

:error
pause
exit 1