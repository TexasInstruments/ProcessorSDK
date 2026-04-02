#!/bin/bash
# ******************************************************************************
# * FILE PURPOSE: SA LLD Builder
# ******************************************************************************
# * FILE NAME: sabuildall.sh
# *
# * DESCRIPTION: 
# *  The batch file cleans and builds the installed SA LLD
# *  Please make sure that the SA LLD Build Environment is setup correctly before 
# *  calling this file. 
# *
# *****************************************************************************

# *****************************************************************************
# Clean all SA LLD libraries
# *****************************************************************************
gmake -C ti/drv/sa     clean

# *****************************************************************************
# Build both BE and LE libraries
# *****************************************************************************
gmake -C ti/drv/sa     all

