::
:: Updates the project's dependencies.
::
@echo off
setlocal

set thisDir=%~dp0
set depDir=%thisDir%..\..\dependencies

:: essentutils
rmdir %depDir%\essentutils /S /Q
set depProj=%thisDir%..\..\..\essentutils
call %depProj%\project\vs\deploy_as_sources.bat %depDir%

:: win32-util
rmdir %depDir%\win32_util /S /Q
set depProj=%thisDir%..\..\..\win32_util
call %depProj%\project\vs\deploy_as_sources.bat %depDir%

:: qconfig
rmdir %depDir%\qconfig /S /Q
set depProj=%thisDir%..\..\..\qconfig
call %depProj%\project\vs\deploy_as_sources.bat %depDir%
