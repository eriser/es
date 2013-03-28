@echo off

call util\env.bat
call util\env_mingw.bat

set HEADER=PROJECT:

echo.
echo %HEADER% es_editor
echo.
cd ..\es_mingw_editor
call doit.upx.bat
if errorlevel 1 goto error
copy Release\prj_upx.exe ..\bin\editor\%PRJ%_editor_mingw.exe
if errorlevel 1 goto error

echo.
echo.
echo %HEADER% es_lib
echo.
cd ..\es_mingw_lib
call doit.bat
if errorlevel 1 goto error
copy libes.a ..\bin\lib\lib%PRJ%_mingw_static.a
if errorlevel 1 goto error

echo.
echo.
echo %HEADER% eswav
echo.
cd ..\eswav_mingw
call doit.upx.bat
if errorlevel 1 goto error
copy Release\eswav_upx.exe ..\bin\eswav\eswav_%PRJ%_mingw.exe
if errorlevel 1 goto error

echo.
echo.
echo %HEADER% ptch
echo.
cd ..\ptch_mingw
call doit.upx.bat
if errorlevel 1 goto error
copy Release\ptch_upx.exe ..\bin\ptch\ptch_%PRJ%_mingw.exe
if errorlevel 1 goto error

echo.
echo.
echo all done!
echo.
pause
goto :EOF

:error
echo ERROR!
pause
