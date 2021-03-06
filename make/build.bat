@echo off

set DEBUGVARS=/Od /Zi
set RELEASEVARS=/O2 /Oi

set code=..\code\main.cpp

set includes=/I "..\deps\includes"

set libs=/link opengl32.lib^
		/LIBPATH:..\deps\libs\ SDL2.lib SDL2main.lib glew32.lib

set options=%DEBUGVARS% %code% %includes% %libs%

mkdir ..\bin
xcopy /D "..\deps\libs\*.dll" "..\bin\"

pushd ..\bin
cl -EHsc %options% /SUBSYSTEM:CONSOLE
popd
