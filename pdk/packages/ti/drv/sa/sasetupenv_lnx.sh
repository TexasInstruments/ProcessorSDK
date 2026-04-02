#!/bin/sh
#******************************************************************************
#* FILE PURPOSE: Environment Setup for building SA LLD on Linux Bash shell
#******************************************************************************
#* FILE NAME: setupenv.sh
#*
#* DESCRIPTION: 
#*  Configures and sets up the Build Environment for SA LLD
#*  Customers are expected to modify this file as per their build environment.
#*
#* Copyright (C) 2012, Texas Instruments, Inc.
#*****************************************************************************

# C66X_GEN_INSTALL_DIR   - C6x Code gen tools install directory
export C6X_GEN_INSTALL_PATH=/opt/ti/C6000CGT7.4.1/

# GNU_ARM_CORTEXA8_GEN_INSTALL_DIR- GNU ARM Cortex-A8 tools install directory
export CROSS_TOOL_INSTALL_PATH=/opt/linaro-2013.03/bin
export CROSS_TOOL_PRFX=arm-linux-gnueabihf-

# SET THE PATH TO  PDK INSTALL PATH (Example as below)
export PDK_INSTALL_PATH=$HOME/ti/pdk_keystone2_3_01_00_00/packages

# PATH TO  SA INSTALL DIR
export SA_INSTALL_DIR=$PWD

echo SA LLD BUILD ENVIRONMENT CONFIGURED
