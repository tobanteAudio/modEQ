@ECHO OFF

CLS
ECHO %cd%
ECHO Building tobanteAudio modEQ...

REM PROJUCER
"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\msbuild.exe" %cd%\3rd_party\JUCE\extras\Projucer\Builds\VisualStudio2017\Projucer.sln /property:Platform=x64 /m

REM MODEQ
%cd%\3rd_party\JUCE\extras\Projucer\Builds\VisualStudio2017\x64\Debug\App\Projucer.exe --resave %cd%\modEQ.jucer
"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\msbuild.exe" %cd%\Builds\VisualStudio2017\modEQ.sln /p:Platform=x64 /p:Configuration=Release /m

ECHO Done!
PAUSE