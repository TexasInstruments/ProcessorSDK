@REM ******************************************************************************
@REM * FILE PURPOSE: Environment Setup for building SA LLD
@REM ******************************************************************************
@REM * FILE NAME: sasetupenv.bat
@REM *
@REM * DESCRIPTION: 
@REM *  Configures and sets up the Build Environment for SA LLD
@REM *
@REM *  The batch file expects an optional argument:SALLD_INSTALL_PATH: Location of the SA LLD package.
@REM *   If the argument is not specified the  batch file assumes that the SA LLD is installed in the same location 
@REM *   where the batch file is located and is being executed.
@REM *
@REM * USAGE:
@REM *  sasetupenv.bat "C:/Program Files/Texas Instruments/salld_2_0_0_0/packages" 
@REM *      --- OR ---
@REM *  sasetupenv.bat
@REM *
@REM *  This batch file may be modified per local build environment.
@REM *****************************************************************************

@echo *******************************************************************************
@echo Configuring SA LLD Build Environment
@echo off

@REM *******************************************************************************
@REM ********************** GET PARAMETERS PASSED THROUGH ARGUMENT   ***************
@REM *******************************************************************************
@REM Parameter Validation: Check if the argument was passed to the batch file and
@REM if so we use that else we default to the working directory where the batch 
@REM file was invoked from

set tempVar=%1
IF NOT DEFINED tempVar GOTO noparameter
set SALLD_INSTALL_PATH=%~fs1
goto done1
:noparameter
set SALLD_INSTALL_PATH=%~sdp0
:done1

REM *******************************************************************************
REM ********************** CHECK REQUIRED ENVIRONMENT DEFINES BEGIN ***************
REM *******************************************************************************
if not defined XDC_INSTALL_PATH   @echo "XDC_INSTALL_PATH: XDC NOT CONFIGURED!!!!. REQUIRED FOR XDC BUILD"
if not defined C6X_GEN_INSTALL_PATH  goto HLP_C6X_GEN_INSTALL_PATH
if not defined PDK_INSTALL_PATH   goto HLP_PDK_INSTALL_PATH 

REM *******************************************************************************
REM *************************** OPTIONAL ENVIRONMENT DEFINES **********************
REM *************************** NOT REQUIRED FOR BUILDING SA LLD  *****************
REM *******************************************************************************
REM *******************************************************************************
REM *******************************************************************************
REM *******************************************************************************

REM *******************************************************************************
REM *************************** Build Variables ***********************************
REM *******************************************************************************

REM To re-build the SA LLD, it is necessary to set the following environment variables
REM based on the build environment .


set C6X_GEN_INSTALL_PATH=%C6X_GEN_INSTALL_PATH:\=/%
@REM *******************************************************************************
@REM ** Convert C6X_GEN_INSTALL_PATH to short name and to UNIX STYLE PATH         **
@REM *******************************************************************************
if not defined XDC_INSTALL_PATH goto END_SHORT_CONVERSION
set PATH=%PATH%;%XDC_INSTALL_PATH%\packages\xdc\services\io\release
@REM for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos %C6X_GEN_INSTALL_PATH%') do set C6X_GEN_INSTALL_PATH=%%a 

for /f "tokens=1* delims=" %%a in ('cmd /q/c path2dos %C6X_GEN_INSTALL_PATH%') do set C6X_GEN_INSTALL_PATH=%%a

:END_SHORT_CONVERSION

REM This is the location where the tools are installed. Please ensure that the names
REM are in short format.
REM Path of C66 tools 
set C66CODEGENTOOL=%C6X_GEN_INSTALL_PATH%
set C66X_GEN_INSTALL_DIR=%C6X_GEN_INSTALL_PATH%


REM SA Low Level Driver depends upon the following packages:-
REM     - CSL Package for the CSL PA Register Layer & Base Address
REM These packages should be installed before trying to build the driver else 
REM compilations will fail.

REM CSL should be included at the PDK
set PDK_INSTALL_PATH=%PDK_INSTALL_PATH:\=/%
set CSL_INSTALL_DIR=%PDK_INSTALL_PATH%/packages

REM SA Install path
set SA_INSTALL_DIR=%SALLD_INSTALL_PATH:\=/%

REM # Module build options
set ARGS_XS=%SA_INSTALL_DIR%\ti\drv\sa\xdcargs.xs

REM # Create environment variables for XDC. Modify and enable below variables
REM # for location of xdc and XDCPATH
REM # 
set XDC_INSTALL_DIR=%XDC_INSTALL_PATH%
set xdc=%XDC_INSTALL_DIR%/xdc.exe $*
set XDCPATH=%SA_INSTALL_DIR%;%CSL_INSTALL_DIR%

REM # Windows path
set PATH=%SystemRoot%;%SystemRoot%/system32

REM # Add XDC path
set PATH=%PATH%;%XDC_INSTALL_DIR%;%XDC_INSTALL_DIR%/bin

REM # set XDC PATH
set XDCPATH=%XDCPATH%;%XDC_INSTALL_DIR%/packages

goto SUCCESS

:HLP_C6X_GEN_INSTALL_PATH
@echo "ENVIRONMENT VARIABLE C6X_GEN_INSTALL_PATH: Code Generation Tool NOT CONFIGURED!!!!"
@echo EXAMPLE [DEFAULT LOCATION]: Note the forward slash would be required in windows environment
@echo Example: set C6X_GEN_INSTALL_PATH=c:\ti\ccsv5\tools\compiler\c6000_7.4.2

goto ERROR

:HLP_XDC_INSTALL_DIR
@echo EXAMPLE [DEFAULT LOCATION]:
@echo  set XDC_INSTALL_PATH=C:/Program Files/Texas Instruments/xdctools_3_22_04_46

goto ERROR

:HLP_PDK_INSTALL_DIR
:HLP_PDK_INSTALL_PATH
@echo "ENVIRONMENT VARIABLE PDK_INSTALL_PATH: XDC NOT CONFIGURED!!!!"
@echo EXAMPLE [DEFAULT LOCATION]:
@echo set PDK_INSTALL_PATH=c:\ti\pdk_keystone2_3_01_00_00\packages

goto ERROR

:SUCCESS
@echo on
@echo SA LLD BUILD ENVIRONMENT CONFIGURED
@echo *******************************************************************************
@echo off
goto DONE

:ERROR
@echo on
@echo ERROR CONFIGURING PDK BUILD ENVIRONMENT
@echo *******************************************************************************
:DONE

REM # End of file

