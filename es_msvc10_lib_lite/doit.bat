call ..\bin\util\env_msvc.bat
set NAME=prj
rem %DEVENV% /project %NAME% /build "Debug" %NAME%.sln
%DEVENV% /project %NAME% /build "Debug_MD|Win32" %NAME%.sln
if Errorlevel 1 goto error
%DEVENV% /project %NAME% /build "Debug_MD|x64" %NAME%.sln
if Errorlevel 1 goto error
%DEVENV% /project %NAME% /build "Debug_MT|Win32" %NAME%.sln
if Errorlevel 1 goto error
%DEVENV% /project %NAME% /build "Debug_MT|x64" %NAME%.sln
if Errorlevel 1 goto error
%DEVENV% /project %NAME% /build "Release_MD|Win32" %NAME%.sln
if Errorlevel 1 goto error
%DEVENV% /project %NAME% /build "Release_MD|x64" %NAME%.sln
if Errorlevel 1 goto error
%DEVENV% /project %NAME% /build "Release_MT|Win32" %NAME%.sln
if Errorlevel 1 goto error
%DEVENV% /project %NAME% /build "Release_MT|x64" %NAME%.sln
if Errorlevel 1 goto error
goto :EOF

:error
pause
exit 1

