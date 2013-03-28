@echo off

call util\env.bat
call util\env_mingw.bat

cd ..\es_mingw_editor
call clean.bat
if errorlevel 1 goto error

cd ..\es_mingw_lib
call clean.bat
if errorlevel 1 goto error

cd ..\es_mingw_rt
call clean.bat
if errorlevel 1 goto error

cd ..\eswav_mingw
call clean.bat
if errorlevel 1 goto error

cd ..\ptch_mingw
call clean.bat
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
