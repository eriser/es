@echo off

call util\env.bat
call util\env_msvc.bat

set HEADER=PROJECT:

echo.
echo %HEADER% es_editor_msvc10
echo.
cd ..\es_msvc10_editor
call doit.upx.bat
if errorlevel 1 goto error
if not exist Release_Win32\prj_upx.exe goto error
copy Release_Win32\prj_upx.exe ..\bin\editor\%PRJ%_editor_msvc10_Win32.exe
if errorlevel 1 goto error
if not exist Release_x64\prj.exe goto error
copy Release_x64\prj.exe ..\bin\editor\%PRJ%_editor_msvc10_x64.exe
if errorlevel 1 goto error

echo.
echo.
echo %HEADER% es_lib
echo.
cd ..\es_msvc10_lib
call doit.bat
if errorlevel 1 goto error

if not exist Debug_MD_Win32\prj.lib goto error
copy Debug_MD_Win32\prj.lib ..\bin\lib\%PRJ%_msvc10_MD_static_debug_Win32.lib
if errorlevel 1 goto error
if not exist Debug_MD_x64\prj.lib goto error
copy Debug_MD_x64\prj.lib ..\bin\lib\%PRJ%_msvc10_MD_static_debug_x64.lib
if errorlevel 1 goto error

if not exist Debug_MT_Win32\prj.lib goto error
copy Debug_MT_Win32\prj.lib ..\bin\lib\%PRJ%_msvc10_MT_static_debug_Win32.lib
if errorlevel 1 goto error
if not exist Debug_MT_x64\prj.lib goto error
copy Debug_MT_x64\prj.lib ..\bin\lib\%PRJ%_msvc10_MT_static_debug_x64.lib
if errorlevel 1 goto error

if not exist Release_MD_Win32\prj.lib goto error
copy Release_MD_Win32\prj.lib ..\bin\lib\%PRJ%_msvc10_MD_static_release_Win32.lib
if errorlevel 1 goto error
if not exist Release_MD_x64\prj.lib goto error
copy Release_MD_x64\prj.lib ..\bin\lib\%PRJ%_msvc10_MD_static_release_x64.lib
if errorlevel 1 goto error

if not exist Release_MT_Win32\prj.lib goto error
copy Release_MT_Win32\prj.lib ..\bin\lib\%PRJ%_msvc10_MT_static_release_Win32.lib
if errorlevel 1 goto error
if not exist Release_MT_x64\prj.lib goto error
copy Release_MT_x64\prj.lib ..\bin\lib\%PRJ%_msvc10_MT_static_release_x64.lib
if errorlevel 1 goto error

echo.
echo.
echo %HEADER% eswav
echo.
cd ..\eswav_msvc10
call doit.upx.bat
if errorlevel 1 goto error
if not exist Release_Win32\prj_upx.exe goto error
copy Release_Win32\prj_upx.exe ..\bin\eswav\eswav_%PRJ%_msvc10_Win32.exe
if errorlevel 1 goto error
if not exist Release_x64\prj.exe goto error
copy Release_x64\prj.exe ..\bin\eswav\eswav_%PRJ%_msvc10_x64.exe
if errorlevel 1 goto error

echo.
echo.
echo %HEADER% ptch
echo.
cd ..\ptch_msvc10
call doit.upx.bat
if errorlevel 1 goto error
if not exist Release_Win32\prj_upx.exe goto error
copy Release_Win32\prj_upx.exe ..\bin\ptch\ptch_%PRJ%_msvc10_Win32.exe
if errorlevel 1 goto error
if not exist Release_x64\prj.exe goto error
copy Release_x64\prj.exe ..\bin\ptch\ptch_%PRJ%_msvc10_x64.exe
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
