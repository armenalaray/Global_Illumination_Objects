@echo off

set SourceCodePath=%CD%
set BinRelPath=C:\Source\Radiosity\bin\release
set BinDebPath=C:\Source\Radiosity\bin\debug

set WarnDis= /wd4201 /wd4239 /wd4100 /wd4189 /wd4127 /wd4150 /wd4996 /wd4700
set CompDebOpt= /nologo /EHsc /Zi /Od /MTd /W4 %WarnDis%    
set CompRelOpt= /nologo /EHsc /O2 /MT /W4 %WarnDis%  

set LinkDebOpt= /INCREMENTAL:NO /DEBUG:FULL 
set LinkRelOpt= /INCREMENTAL:NO  


echo ***DEBUG***
pushd ..\..\bin\debug
cl %CompDebOpt% /Fo"pathtracer_debug" %SourceCodePath%\pathtracer.cpp /link %LinkDebOpt% 
popd

echo ***RELEASE***
pushd ..\..\bin\release
cl %CompRelOpt% /Fo"pathtracer_release" %SourceCodePath%\pathtracer.cpp /link %LinkRelOpt% 
popd

pushd ..\..\out\data
%BinRelPath%\pathtracer_release.exe > out.ppm
start C:\AlexPrograms\OpenSeeIt_v0.47.16.4\OpenSeeIt.exe out.ppm
popd

GOTO RADIOSITY

echo ***DEBUG***
pushd ..\..\bin\debug
cl %CompDebOpt% /Fo"radiosity_debug" %SourceCodePath%\radiosity.cpp /link %LinkDebOpt% 
popd

echo ***RELEASE***
pushd ..\..\bin\release
cl %CompRelOpt% /Fo"radiosity_release" %SourceCodePath%\radiosity.cpp /link %LinkRelOpt% 
popd

pushd ..\..\out\data
REM %BinRelPath%\radiosity_release.exe
REM for %%v in ("..\..\out\data\*_matrix.ppm") do start "C:\AlexPrograms\OpenSeeIt_v0.47.16.4\OpenSeeIt.exe" "%%v"
REM popd

:RADIOSITY

GOTO NOISE_TEST

echo ***DEBUG***
pushd ..\..\bin\debug
cl %CompDebOpt% /Fo"noise_test" %SourceCodePath%\noise_test.cpp /link %LinkDebOpt% 
popd

pushd ..\..\out\data
%BinDebPath%\noise_test.exe
popd

for %%v in ("..\..\out\data\perlin_noise.ppm") do start "C:\AlexPrograms\OpenSeeIt_v0.47.16.4\OpenSeeIt.exe" "%%v"

:NOISE_TEST

GOTO VOLUME_TEST

echo ***DEBUG***
pushd ..\..\bin\debug
cl %CompDebOpt% /Fo"volume_test" %SourceCodePath%\volume_test.cpp /link %LinkDebOpt% 
popd

pushd ..\..\out\data
%BinDebPath%\volume_test.exe
popd

for %%v in ("..\..\out\data\volume.ppm") do start "C:\AlexPrograms\OpenSeeIt_v0.47.16.4\OpenSeeIt.exe" "%%v"

:VOLUME_TEST


GOTO MATRIX_TEST

echo ***DEBUG***
pushd ..\..\bin\debug
cl %CompDebOpt% /Fo"matrix" %SourceCodePath%\matrix_perf_test.cpp /link %LinkDebOpt% 
popd

pushd ..\..\out\data
%BinDebPath%\matrix.exe
popd

REM for %%v in ("..\..\out\data\volume.ppm") do start "C:\AlexPrograms\OpenSeeIt_v0.47.16.4\OpenSeeIt.exe" "%%v"

:MATRIX_TEST

GOTO ESTIMATE_PI

echo ***DEBUG***
pushd ..\..\bin\debug
cl %CompDebOpt% /Fo"pi" %SourceCodePath%\montecarlo.cpp /link %LinkDebOpt% 
popd

pushd ..\..\out\data
%BinDebPath%\pi.exe 64
popd

REM for %%v in ("..\..\out\data\volume.ppm") do start "C:\AlexPrograms\OpenSeeIt_v0.47.16.4\OpenSeeIt.exe" "%%v"

:ESTIMATE_PI

GOTO LT

echo ***DEBUG***
pushd ..\..\bin\debug
cl %CompDebOpt% /Fo"lt" %SourceCodePath%\lt.cpp /link %LinkDebOpt% 
popd

pushd ..\..\out\data
%BinDebPath%\lt.exe 
popd

REM for %%v in ("..\..\out\data\SSS_*.ppm") do start "C:\AlexPrograms\OpenSeeIt_v0.47.16.4\OpenSeeIt.exe" "%%v"

:LT



