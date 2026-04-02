#!/bin/sh
#******************************************************************************
#* FILE PURPOSE: Environment Setup for building SA LLD
#******************************************************************************
#* FILE NAME: setupenv.sh
#*
#* DESCRIPTION: 
#*  Configures and sets up the Build Environment for SA LLD
#*  Customers are expected to modify this file as per their build environment.
#*
#*****************************************************************************

# Update the install directory location from here
if [ ! -n "$XDC_INSTALL_PATH" ]; then
  export XDC_INSTALL_PATH=../DEP_TOOLS/xdctools_3_25_02_70
  echo setting XDC_INSTALL_PATH to user set path: $XDC_INSTALL_PATH
fi

# PATH for the XDC Plugin gen path
if [ ! -n "$XDCPLUGIN_INSTALL_DIR" ]; then
  export XDCPLUGIN_INSTALL_DIR=../DEP_TOOLS/xdc_eclipse_plugin_gen/20091203  
  echo setting XDCPLUGIN_INSTALL_DIR to user set path: $XDCPLUGIN_INSTALL_DIR
fi

# c66x Tool chain path
if [ ! -n "$C6X_GEN_INSTALL_PATH" ]; then
  export C6X_GEN_INSTALL_PATH=$HOME/ti/TI_CGT_C6000_7.4.2
  echo setting C6X_GEN_INSTALL_PATH to user set path: $C6X_GEN_INSTALL_PATH  
fi
export C66CODEGENTOOL=$C6X_GEN_INSTALL_PATH
export XDCCGROOT=$C6X_GEN_INSTALL_PATH

# export cgxml install path
if [ ! -n "$CGXML_INSTALL_PATH" ]; then
  export CGXML_INSTALL_PATH=$HOME/ti/cg_xml
  echo setting CGXML_INSTALL_PATH to user set path: $CGXML_INSTALL_PATH  
fi

# ARM Cortex-A8 Tool chain path
if [ ! -n "$CROSS_TOOL_INSTALL_PATH" ]; then
  export CROSS_TOOL_INSTALL_PATH=/opt/linaro-2013.03/bin
  export CROSS_TOOL_PRFX=arm-linux-gnueabihf-
  echo setting CROSS_TOOL_INSTALL_PATH to user set path: $CROSS_TOOL_INSTALL_PATH  
fi
export GCARMV7ACGTOOL=$CROSS_TOOL_INSTALL_PATH
export GCARMV7ALONGNAME=$CROSS_TOOL_PRFX

# SA LLD install directory dir
if [ ! -n "$SA_INSTALL_DIR" ]; then
  export SA_INSTALL_DIR=../../../../packages
  echo setting SA_INSTALL_DIR to user set path: $SA_INSTALL_DIR  
fi

# Set path to root folder of CSL (i.e packages directory of the PDK)
if [ ! -n "$PDK_INSTALL_PATH" ]; then
  export PDK_INSTALL_PATH=../DEP_TOOLS/mcsdk_3_00_00_12/pdk_keystone2_3_00_01_12/packages
  echo setting PDK_INSTALL_PATH to user set path: $PDK_INSTALL_PATH  
fi

if [ ! -n "$INSTALL_JAMMER_PATH" ]; then
  export INSTALL_JAMMER_PATH=$HOME/installjammer
  echo setting INSTALL_JAMMER_PATH to user set path: $INSTALL_JAMMER_PATH  
fi

# Verify setup paths (Please do not change anything from here 
export DIR_NOT_EXIST=
if ! [ -d "$XDC_INSTALL_PATH" ]; then
export DIR_NOT_EXIST=FALSE
echo XDC_INSTALL_PATH:$XDC_INSTALL_PATH does not exist!!!
fi

if ! [ -d "$C6X_GEN_INSTALL_PATH" ]; then
export DIR_NOT_EXIST=FALSE
echo C6X_GEN_INSTALL_PATH:$C6X_GEN_INSTALL_PATH does not exist!!!
fi
if ! [ -d "$SA_INSTALL_DIR" ]; then
export DIR_NOT_EXIST=FALSE
echo SA_INSTALL_DIR:$SA_INSTALL_DIR does not exist!!!
fi

if ! [ -d "$CROSS_TOOL_INSTALL_PATH" ]; then
export DIR_NOT_EXIST=FALSE
echo CROSS_TOOL_INSTALL_PATH:$CROSS_TOOL_INSTALL_PATH does not exist!!!
fi

if ! [ -d "$PDK_INSTALL_PATH" ]; then
export DIR_NOT_EXIST=FALSE
echo PDK_INSTALL_PATH:$exitINSTALL_PATH does not exist !!!
fi

if ! [ -d "$INSTALL_JAMMER_PATH" ]; then
export DIR_NOT_EXIST=FALSE
echo INSTALL_JAMMER_PATH:$INSTALL_JAMMER_PATH does not exist !!!
fi

if ! [ -d "$CGXML_INSTALL_PATH" ]; then
export DIR_NOT_EXIST=FALSE
echo CGXML_INSTALL_PATH:$CGXML_INSTALL_PATH does not exist !!!
fi

if [ "$DIR_NOT_EXIST" == "" ];then
	echo SA LLD BUILD ENVIRONMENT SUCCESFULLY CONFIGURED
  # Any other components you could add as
  # XDCPATH=<your component>/packages;...
  export XDCPATH="$XDC_INSTALL_PATH/packages;$PDK_INSTALL_PATH;$SA_INSTALL_DIR;$XDCPLUGIN_INSTALL_DIR"
  export PATH=$PATH:$XDC_INSTALL_PATH:$INSTALL_JAMMER_PATH:$CROSS_TOOL_INSTALL_PATH:$CGXML_INSTALL_PATH/bin:$C6X_GEN_INSTALL_PATH/bin:$SA_LLD_REL/webgen/exports	
else
  echo SA LLD BUILD ENVIRONMENT NOT CONFIGURED 
fi
