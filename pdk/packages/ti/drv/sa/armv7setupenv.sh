#!/bin/sh

# EDIT below variables as per MCSDK installation locations and CROSS TOOL needed to 
# compile the applications

# PDK package install path
export PDK_INSTALL_PATH=$HOME/MCSDK_3.1/pdk_keystone2_3_01_00_00/packages
echo "PDK_INSTALL_PATH set to $PDK_INSTALL_PATH"

# Directory where the (PA/QMSS/CPPI) LLD libraries are available
export ARMV7LIBDIR=$PDK_INSTALL_PATH/../lib/armv7
echo "ARMV7LIBDIR set to $ARMV7LIBDIR"

# Export the device 
export DEVICE=k2h

# Default example applications do not depend on the dynamic libraries
# Un comment below line to have example applications depend on dynamic libraries
# export USEDYNAMIC_LIB="yes"

# Export CORE type
export CORE=armv7
echo "CORE set to $CORE"

# ARM cross tool executable path
export CROSS_TOOL_INSTALL_PATH=/opt/linaro-2013.03/bin
echo "CROSS_TOOL_INSTALL_PATH set to $CROSS_TOOL_INSTALL_PATH"

# ARM cross tool prefix 
export CROSS_TOOL_PRFX=arm-linux-gnueabihf-

# DEBUG FLAG: Set this to blank for a non-debug build 
export DEBUG_FLAG=-g

# SA_INSTALL_PATH
export SA_INSTALL_PATH=$PWD
echo "SA_INSTALL_PATH set to $SA_INSTALL_PATH"

#Default SA Libaray location
export ARMV7SALIBDIR=$SA_INSTALL_PATH/ti/drv/sa/lib/armv7

# Default Output directory for generated binaries and objects
export ARMV7SABINDIR=$SA_INSTALL_PATH/../bin/$DEVICE/armv7
export ARMV7SAOBJDIR=$SA_INSTALL_PATH/../obj/$DEVICE/armv7

# Default configuration, 3gpp support is disabled 
# Please modify below variable to enable  sa Examples to be functional 
# along  with SA 3GPP Enabler for ARM user space. 
# Please make sure the SA 3gpp enabler is installed under SA package 
# before enabling the support for SA examples 
#
# Example to enable 3gpp support in the examples is as below:
# export SA_EXAMPLES_3GPP_SUPPORT=ENABLE
export SA_EXAMPLES_3GPP_SUPPORT=DISABLE

echo "Build Environment configured. Run make to build targets, for example, (make -C ti/drv/sa -f makefile_armv7 all)"
