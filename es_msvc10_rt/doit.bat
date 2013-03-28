call ..\bin\util\env_msvc.bat
set NAME=prj
%DEVENV% /project %NAME% /rebuild "Release|Win32" %NAME%.sln
if Errorlevel 1 goto error
if not "%1" == "" goto :EOF rem skip x64!
%DEVENV% /project %NAME% /rebuild "Release|x64" %NAME%.sln
if Errorlevel 1 goto error
goto :EOF

:error
pause
exit 1
