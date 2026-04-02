#REM ******************************************************************************
#REM * FILE PURPOSE: SA Unit Test and Example Project Creator
#REM ******************************************************************************
#REM * FILE NAME: saProjectCreate.sh
#REM *
#REM * DESCRIPTION: 
#REM *  The script file is used to create the test and example projects of all
#REM *  components under SA. These projects are available in the specified 
#REM *  workspace.
#REM *
#REM * USAGE:
#REM *  saProjectCreate.sh "$HOME\ti\salld_keystone2_3_01_00_00\packages"
#REM *      --- OR ---
#REM *  saProjectCreate.sh
#REM *
#REM *
#REM * Copyright (C) 2011-2013, Texas Instruments, Inc.
#REM *****************************************************************************
#echo OFF
export PDK_SHORT_NAME=$PWD
#*****************************************************************************
#*****************************************************************************
#                         Customer Modifiable Section
# *****************************************************************************
# *****************************************************************************
#macros.ini location
export MACROS_FILE=$PDK_SHORT_NAME/macros.ini
# This is to control the CCS version specific project create command
# Set to 'no' when using CCSv5 or set to 'yes' when using CCSv4
export IS_CCS_VERSION_4=no
# Set to 'no' when using QT, EVM, VDB, or other hardware. Set to 'yes' only when using the simulator.
export IS_SIMULATOR_SUPPORT_NEEDED=no
#Install Location for CCS. Ensure the PATH here is in compliance with the 'IS_CCS_VERSION_4' variable
# defined above.
export CCS_INSTALL_PATH=$HOME/ti/ccsv5
#Workspace where the PDK projects will be created.
export MY_WORKSPACE=$PDK_SHORT_NAME/MySaProjects
# This is Endianess of the Projects being created.
# Valid Values are 'little' and 'big'
export ENDIAN=little
# This is the format of the executable being created
# Valid Values are 'ELF' and 'COFF'
export OUTPUT_FORMAT=ELF
# Version of CG-Tools
export CGT_VERSION=7.4.2
# Version of XDC
export XDC_VERSION=3.25.02.70
# Version of BIOS
export BIOS_VERSION=6.35.04.50
# Version of the IPC
export IPC_VERSION=3.00.02.26
# Version of the PDK
export PDK_VERSION=3.01.00.00
# PDK Part Number
export PDK_PARTNO=TCI6634
# Version of the SALLD
export SA_VERSION=03.00.00.03
# RTSC Platform Name
export RTSC_PLATFORM_NAME=ti.platforms.simKepler
# RTSC Target 
# - Please ensure that you select this taking into account the
#   OUTPUT_FORMAT and the RTSC_PLATFORM_NAME 
if [ "$ENDIAN" == "big" ]; then
   export RTSC_TARGET=ti.targets.elf.C66_big_endian
   echo "RTSC_TARGET configured for elf c66 big endian mode"
else
   export RTSC_TARGET=ti.targets.elf.C66
   echo "RTSC_TARGET configured for elf c66 little endian mode"
fi
# *****************************************************************************
# *****************************************************************************
#                 Please do NOT change anything below this
# *****************************************************************************
# *****************************************************************************

# export auto create command by default for use with CCSv5
export AUTO_CREATE_COMMAND=eclipse/ccstudio

# If is CCS version 4 then set auto create command for use with CCSv4
#If .%IS_CCS_VERSION_4% == .yes set AUTO_CREATE_COMMAND=eclipse\jre\bin\java -jar %CCS_INSTALL_PATH%\eclipse\startup.jar

# export project for Silicon or QT by default
export SIMULATOR_SUPPORT_DEFINE=yes

# If simulator support is needed then set the define
if [ "$IS_SIMULATOR_SUPPORT_NEEDED" == "yes" ];then
  export SIMULATOR_SUPPORT_DEFINE=-ccs.setCompilerOptions "--define SIMULATOR_SUPPORT"
fi

echo *****************************************************************************
echo Detecting UnitTest Projects in SA LLD package and importing them in the workspace $MY_WORKSPACE

ccs_create()
{
	echo found $1
	textfile=${1#*/}
	$CCS_INSTALL_PATH/$AUTO_CREATE_COMMAND -nosplash -data $MY_WORKSPACE -application com.ti.ccstudio.apps.projectCreate -ccs.name ${textfile%.*} -ccs.outputFormat $OUTPUT_FORMAT -ccs.device com.ti.ccstudio.deviceModel.C6000.GenericC66xxDevice -ccs.endianness $ENDIAN -ccs.kind executable -ccs.cgtVersion $CGT_VERSION -rtsc.xdcVersion $XDC_VERSION -rtsc.enableDspBios -rtsc.biosVersion $BIOS_VERSION -rtsc.buildProfile "debug" -rtsc.products "com.ti.sdo.edma3:$EDMA_VERSION;com.ti.rtsc.IPC:$IPC_VERSION;com.ti.rtsc.SYSBIOS:$BIOS_VERSION;ti.pdk:$PDK_VERSION" -rtsc.platform "$RTSC_PLATFORM_NAME" -rtsc.target $RTSC_TARGET -ccs.rts libc.a -ccs.args $textfile \;
	cp $MACROS_FILE $MY_WORKSPACE/${textfile%.*}
}		

export -f ccs_create

find -name "*roject.txt" -execdir bash -c 'ccs_create {}' \;
