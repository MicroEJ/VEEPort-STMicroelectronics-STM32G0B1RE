@echo off

REM Copyright 2019-2023 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'set_project_env.bat' implementation for IAR Embedded Workbench.

REM 'set_project_env' is responsible for
REM - checking the availability of required environment variables 
REM - setting project local variables for 'build.bat' and 'run.bat' 


SET IAREW_INSTALLATION_DIR=C:\Program Files (x86)\IAR Systems\Embedded Workbench 8.4_2

REM Set the project variables required by the build script for IAR Embedded Workbench
REM IAREW project directory that contains the project file .ewp (e.g.: %~dp0\..\)
SET IAREW_PROJECT_DIR=%~dp0..\EWARM
REM IAREW project file name without the extension .ewp (e.g.: Project)
SET IAREW_PROJECT_NAME=application
REM IAREW project configuration (e.g.: Debug or Release)
SET IAREW_PROJECT_CONFIGURATION=Release
REM IAREW out file name  (file name defined in IAR project option without .out)
SET IAREW_OUT_FILENAME=application

REM Device macro file relative to IAREW_INSTALLATION_DIR (e.g.: arm\config\debugger\NXP\LPC5460x.dmac)
SET IAREW_PROJECT_DEVICE_MACRO=arm\config\debugger\ST\STM32G0xx.dmac

REM Device flash loader file relative to IAREW_INSTALLATION_DIR (e.g.: arm\config\flashloader\NXP\FlashNXPLPC5460xM4F512K.board)
SET IAREW_PROJECT_FLASH_LOADER=arm\config\flashloader\ST\FlashSTM32G0BxxE.board

REM Path of the the binaire file to flash into the board
SET IAREW_PROJECT_EXECUTABLE_FILE="%IAREW_PROJECT_DIR%\%IAREW_PROJECT_CONFIGURATION%\Exe\%IAREW_OUT_FILENAME%.out"

ECHO IAREW_INSTALLATION_DIR=%IAREW_INSTALLATION_DIR%
ECHO IAREW_PROJECT_DIR=%IAREW_PROJECT_DIR%
ECHO IAREW_PROJECT_NAME=%IAREW_PROJECT_NAME%
ECHO IAREW_PROJECT_CONFIGURATION=%IAREW_PROJECT_CONFIGURATION%
exit /B 0