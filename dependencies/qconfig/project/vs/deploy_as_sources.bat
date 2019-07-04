::
:: Deploys project and sources of the qconfig library to a given
:: directory. Will create a subdirectory for the library.
:: Example: > .\deploy_as_sources.bat C:\libs
::
@echo off
setlocal

set outDir=%1
set thisDir=%~dp0

set src=%thisDir%..\..
set dest=%outDir%\qconfig

:: Deploy code.
mkdir %dest%
robocopy %src% %dest% *.h
robocopy %src% %dest% *.cpp

:: Deploy project.
set from=%src%\project\vs
set to=%dest%\project\vs
mkdir %to%
robocopy %from% %to% qconfig.vcxproj
robocopy %from% %to% qconfig.vcxproj.filters
robocopy %from% %to% deploy_as_sources.bat

:: Deploy dependencies by calling deployment scripts of each dependency.
call %src%\dependencies\essentutils\project\vs\deploy_as_sources.bat %dest%\dependencies
call %src%\dependencies\win32_util\project\vs\deploy_as_sources.bat %dest%\dependencies
