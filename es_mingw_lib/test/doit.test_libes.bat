@echo off

if not exist ../libes.a goto error
g++ -Wall -I. -L./.. -D__WINDOWS__ -O3 -o test_libes test_libes.cpp -s -les -lwinmm -ldsound && test_libes
pause
goto :EOF

:error
echo build libes.a first!
echo.
pause
