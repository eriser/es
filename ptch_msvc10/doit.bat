call ..\bin\util\env_msvc.bat
set NAME=prj
%DEVENV% /project %NAME% /build "Release|Win32" %NAME%.sln
if Errorlevel 1 goto error
%DEVENV% /project %NAME% /build "Release|x64" %NAME%.sln
if Errorlevel 1 goto error
goto :EOF

:error
pause
exit 1
