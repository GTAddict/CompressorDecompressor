@echo off

REM Set the path for MSBuild
CD %~dp0
CALL Helper\VsMSBuildCmd.bat

ECHO Building VS project...
MSBuild CompressorDecompressor\CompressorDecompressor.sln /t:Rebuild /m /nr:false /ds /p:Configuration=Release
ECHO Finished building VS project.