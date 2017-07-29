@echo off

CD %~dp0
CD CompressorDecompressor\Release

SET /A START_RANGE = 5
SET /A END_RANGE = 16
SET /A UNCOMPRESSED = 1

ECHO Running compressor...
SETLOCAL ENABLEDELAYEDEXPANSION
FOR /L %%X IN (%START_RANGE%, 1, %END_RANGE%) DO (
	ECHO Compressing with bit depth %%X...
	CALL "CompressorCmd.exe" "..\data\verts.txt" "..\data\verts%%X.bin" "%%X" "!UNCOMPRESSED!"
	SET /A UNCOMPRESSED = 0
)
ECHO Finished compression.

ECHO Deleting old vertex data...
CD ..\data
DEL verts.uncompressed /F /S > NUL
REN "verts%START_RANGE%.uncompressed" "verts.uncompressed"

ECHO Running decompressor...
ECHO Deleting RMS output data...
COPY /Y nul: RMSData.txt
CD ..\Release
FOR /L %%X IN (%START_RANGE%, 1, %END_RANGE%) DO (
	ECHO Uncompressing and calculating RMS for data with bit depth %%X...
	CALL "DecompressorCmd.exe" "..\data\verts%%X.bin" "..\data\verts.uncompressed" >> ..\data\RMSData.txt
)
ECHO Finished decompression.

ECHO Generating excel file...
CD ..\Data
DEL *.xlsx /F /S > NUL
SET "EXCEL_FILE=Analysis.xlsx"
FOR /F "delims=" %%F IN ("%EXCEL_FILE%") DO SET "EXCEL_FILE=%%~fF"
CD ..\Release
CALL "ExcelWriter.exe" "..\data\RMSData.txt" "%EXCEL_FILE%"

ECHO Finished process.