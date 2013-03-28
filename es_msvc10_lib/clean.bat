cd
call ..\bin\util\env_msvc.bat
set NAME=prj
%DEVENV% /project %NAME% /clean "Debug_MD|Win32" %NAME%.sln
if Errorlevel 1 goto error
if exist Debug_MD_Win32 rmdir /s /q Debug_MD_Win32
%DEVENV% /project %NAME% /clean "Debug_MD|x64" %NAME%.sln
if Errorlevel 1 goto error
if exist Debug_MD_x64 rmdir /s /q Debug_MD_x64
%DEVENV% /project %NAME% /clean "Debug_MT|Win32" %NAME%.sln
if Errorlevel 1 goto error
if exist Debug_MT_Win32 rmdir /s /q Debug_MT_Win32
%DEVENV% /project %NAME% /clean "Debug_MT|x64" %NAME%.sln
if Errorlevel 1 goto error
if exist Debug_MT_x64 rmdir /s /q Debug_MT_x64
%DEVENV% /project %NAME% /clean "Release_MD|Win32" %NAME%.sln
if Errorlevel 1 goto error
if exist Release_MD_Win32 rmdir /s /q Release_MD_Win32
%DEVENV% /project %NAME% /clean "Release_MD|x64" %NAME%.sln
if Errorlevel 1 goto error
if exist Release_MD_x64 rmdir /s /q Release_MD_x64
%DEVENV% /project %NAME% /clean "Release_MT|Win32" %NAME%.sln
if Errorlevel 1 goto error
if exist Release_MT_Win32 rmdir /s /q Release_MT_Win32
%DEVENV% /project %NAME% /clean "Release_MT|x64" %NAME%.sln
if Errorlevel 1 goto error
if exist Release_MT_x64 rmdir /s /q Release_MT_x64
goto :EOF

:error
pause
exit 1

