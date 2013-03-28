cd
call ..\bin\util\env_msvc.bat
set NAME=prj
%DEVENV% /project %NAME% /clean "Debug|Win32" %NAME%.sln
if Errorlevel 1 goto error
if exist Debug_Win32 rmdir /s /q Debug_Win32
%DEVENV% /project %NAME% /clean "Debug|x64" %NAME%.sln
if Errorlevel 1 goto error
if exist Debug_x64 rmdir /s /q Debug_x64
%DEVENV% /project %NAME% /clean "Release|Win32" %NAME%.sln
if Errorlevel 1 goto error
if exist Release_Win32 rmdir /s /q Release_Win32
%DEVENV% /project %NAME% /clean "Release|x64" %NAME%.sln
if Errorlevel 1 goto error
if exist Release_x64 rmdir /s /q Release_x64
goto :EOF

:error
pause
exit 1

