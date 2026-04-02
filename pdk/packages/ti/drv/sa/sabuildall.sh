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
# Procedure to build using XDC. Obsolete
# *****************************************************************************

export SA_INSTALL_DIR="$SALLD_INSTALL_PATH"
export CSL_INSTALL_DIR="$PDK_INSTALL_PATH/packages"
export C66X_GEN_INSTALL_DIR=\"$C6X_GEN_INSTALL_PATH\"

# *****************************************************************************
# Clean all SA LLD libraries
# *****************************************************************************
gmake -C ti/drv/sa    clean

# *****************************************************************************
# Build both BE and LE libraries
# *****************************************************************************
gmake -C ti/drv/sa    all

# *****************************************************************************
#  DONE
# *****************************************************************************
