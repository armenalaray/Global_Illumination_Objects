@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

set prjdir=%CD%

set WarnDis= /wd4201 /wd4239 /wd4100 /wd4189 /wd4127 /wd4150 /wd4996 /wd4700
set CompDebOpt= /c /nologo /EHsc /Zi /Od /MTd /W4 %WarnDis%    
set CompRelOpt= /nologo /EHsc /O2 /MT /W4 %WarnDis%  

set LinkDebOpt= /NOLOGO /INCREMENTAL:NO /DEBUG:FULL  
set LinkRelOpt= /NOLOGO /INCREMENTAL:NO  

echo ***DEBUG***
pushd bin\debug
set objs=

for %%v in ("%prjdir%\source\*.cpp") do cl %CompDebOpt% %%v
for %%v in ("%prjdir%\bin\debug\*.obj") do set objs=!objs! %%v

LINK /OUT:main.exe %LinkDebOpt% %objs%

popd

REM echo ***RELEASE***
REM pushd ..\..\bin\release
REM cl %CompRelOpt% /Fo"pathtracer_release" %SourceCodePath%\pathtracer.cpp /link %LinkRelOpt% 
REM popd

REM pushd ..\..\out\data
REM %BinRelPath%\pathtracer_release.exe > out.ppm
REM start C:\AlexPrograms\OpenSeeIt_v0.47.16.4\OpenSeeIt.exe out.ppm
REM popd
