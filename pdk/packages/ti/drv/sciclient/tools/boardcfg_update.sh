#!/bin/bash
#
# Utility script to integrate updated BoardCfg RM to PDK.
#
# Usage : boardcfg_update.sh <BOARD> --boardcfg=path/to/boardcfg/rm/c/file
# 
# Examples:- 
#  ./boardcfg_update.sh j721e_evm --boardcfg=path/to/j721e/sciclient/defaultBoardcfg/rm/c/file
#  ./boardcfg_update.sh j7200_evm --boardcfg=path/to/j7200/sciclient/defaultBoardcfg/rm/c/file
#  ./boardcfg_update.sh j721s2_evm --boardcfg=path/to/j721s2/sciclient/defaultBoardcfg/rm/c/file
#  ./boardcfg_update.sh j784s4_evm --boardcfg=path/to/j784s4/sciclient/defaultBoardcfg/rm/c/file

################################################################################
# Parse CLI arguments
board=$1
for i in "$@"; do
case $i in
    -b=*|--boardcfg=*) # Path to BoardCfg RM .c file
        boardcfg_rm_file="${i#*=}"
        shift
        ;;  
    -h|--help)
      echo "Usage: $0 \<BOARD\> --boardcfg_rm=path/to/boardcfg/rm/c/file"
      echo 
      echo "Supported BOARDs:-"
      echo "- j721e_evm"
      echo "- j7200_evm"
      echo "- j721s2_evm"
      echo "- j784s4_evm"
      echo ""
      echo "BoardCfg RM file name should be \"sciclient_defaultBoardcfg_rm.c\""
      exit 0
      ;;
    -*) # Invalid flag
        echo "!!!WARNING!!! - IGNORING INVALID FLAG: $i"
        shift
        ;;
esac
done

################################################################################
#Check CLI arguments
if [ "${board}" != "j721e_evm" ]  && 
   [ "${board}" != "j7200_evm" ]  && 
   [ "${board}" != "j721s2_evm" ]  && 
   [ "${board}" != "j784s4_evm" ]; then
    echo "!!!ERROR!!! - Invalid BOARD: ${board}"
    exit 1
fi

if [ "${boardcfg_rm_file}" == "" ] || [ ! -f ${boardcfg_rm_file} ]; then
    echo "!!!ERROR!!! - Invalid BoardCfg RM File: ${boardcfg_rm_file}"
    exit 1
fi

boardcfg_rm_file_basename=$(basename ${boardcfg_rm_file})

if [ "${boardcfg_rm_file_basename}" != "sciclient_defaultBoardcfg_rm.c" ] ; then
    echo "!!!ERROR!!! - Invalid BoardCfg RM File: ${boardcfg_rm_file_basename}"
    echo " ---------------------------------------------------------------------"
    echo " | BoardCfg RM file name should be \"sciclient_defaultBoardcfg_rm.c\"  |"
    echo " ---------------------------------------------------------------------"
    exit 1
fi

################################################################################
# Specify paths relative to script
export SCRIPT_DIR=$(cd "$( dirname "${BASH_SOURCE[0]}")" && pwd )
export SCI_CLIENT_DIR=$(cd "$SCRIPT_DIR/.." && pwd )
export ROOTDIR=$(cd "$SCI_CLIENT_DIR/../../.." && pwd )

#Set other variables
IFS='_' read -ra board_strip <<< "${board}"
soc=${board_strip[0]}

case ${soc} in
    "j721e")
        soc_sci_ver=V1
        shift
        ;;
    "j7200")
        soc_sci_ver=V2
        shift
        ;;
    "j721s2")
        soc_sci_ver=V4
        shift
        ;;
    "j784s4")
        soc_sci_ver=V6
        shift
        ;;
esac

############################################################################################
# Check-in BoardCFG RM file to PDK Sciclent directory
echo "Copying ${boardcfg_rm_file} to $SCI_CLIENT_DIR/soc/${soc_sci_ver}/sciclient_defaultBoardcfg_rm.c"
cp ${boardcfg_rm_file} $SCI_CLIENT_DIR/soc/${soc_sci_ver}/sciclient_defaultBoardcfg_rm.c
echo "Copy Done!"

############################################################################################
# Build sciclient binaries
cd $ROOTDIR/ti/build

make -j -s sciclient_boardcfg BOARD=${board}
make -j -s sciclient_boardcfg BOARD=${board} BUILD_HS=yes
make -j -s pdk_libs BOARD=${board}
make -j -s sciclient_ccs_init_clean BOARD=${board}
make -j -s sciclient_ccs_init BOARD=${board}
make -j -s sciserver_testapp_freertos_clean BOARD=${board}
make -j -s sciserver_testapp_freertos BOARD=${board}
cp $ROOTDIR/ti/binary/sciclient_ccs_init/bin/${soc}/sciclient_ccs_init_mcu1_0_release.xer5f $SCI_CLIENT_DIR/tools/ccsLoadDmsc/${soc}/
cp $ROOTDIR/ti/binary/sciserver_testapp_freertos/bin/${soc}/sciserver_testapp_freertos_mcu1_0_release.xer5f $SCI_CLIENT_DIR/tools/ccsLoadDmsc/${soc}/
cp $ROOTDIR/ti/binary/sciserver_testapp_freertos/bin/${soc}/sciserver_testapp_freertos_mcu1_0_release.rprc $SCI_CLIENT_DIR/tools/ccsLoadDmsc/${soc}/
if [ "${board}" != "j784s4_evm" ]; then
    make -j -s sciserver_testapp_safertos_clean BOARD=${board}
    make -j -s sciserver_testapp_safertos BOARD=${board}
    cp $ROOTDIR/ti/binary/sciserver_testapp_safertos/bin/${soc}/sciserver_testapp_safertos_mcu1_0_release.xer5f $SCI_CLIENT_DIR/tools/ccsLoadDmsc/${soc}/
    cp $ROOTDIR/ti/binary/sciserver_testapp_safertos/bin/${soc}/sciserver_testapp_safertos_mcu1_0_release.rprc $SCI_CLIENT_DIR/tools/ccsLoadDmsc/${soc}/
fi
if [ "${board}" = "j784s4_evm" ] || [ "${board}" = "j721s2_evm" ] [ "${board}" = "j7200_evm" ]; then
    make -j -s sciclient_boardcfg_combined BOARD=${board}
fi

cd -

##############
echo "DONE"
