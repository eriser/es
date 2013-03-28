@echo off

call util\env.bat
call util\env_msvc.bat

cd ../es_msvc10_editor
call clean.bat
if errorlevel 1 goto error
echo.

cd ../es_msvc10_lib
call clean.bat
if errorlevel 1 goto error
echo.

cd ../es_msvc10_lib_lite
call clean.bat
if errorlevel 1 goto error
echo.

cd ../es_msvc10_rt
call clean.bat
if errorlevel 1 goto error
echo.

cd ../eswav_msvc10
call clean.bat
if errorlevel 1 goto error
echo.

cd ../ptch_msvc10
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
