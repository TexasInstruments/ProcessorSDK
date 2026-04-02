#!/bin/bash

# User specifies location of HLOS binaries and the specific OS
##############################################################

# BOARD options are: j721e_evm j7200_evm j721s2_evm j784s4_evm
BOARD=

# OS option: linux
OS=linux

##############################################################

# TI packages paths needed for generation of the appimages

PDK_INSTALL_PATH=${PWD}/../../../../../../..
# To get rid of ../../ in the path go to PDK_INSTALL_PATH and reassign PDK_INSTALL_PATH as PWD
pushd ${PWD}
cd ${PDK_INSTALL_PATH}
PDK_INSTALL_PATH=${PWD}
popd

SBL_PATH="${PDK_INSTALL_PATH}/ti/boot/sbl"
MULTICORE_APPIMAGE_GEN_TOOL_PATH="${SBL_PATH}/tools/multicoreImageGen/bin"
SBL_OUT2RPRC_GEN_TOOL_PATH="${SBL_PATH}/tools/out2rprc/bin"
BOOTAPP_BIN_PATH="${SBL_PATH}/tools/BootApp_binaries/${OS}"
LDS_PATH=${PWD}/${OS}

usage()
{
    echo
	echo "constructappimageshlos.sh => Utility script to generate appimages to boot Linux from BootApp"
	echo "Usage:"
	echo "  ./constructappimageshlos.sh --board=<BOARD>"
    echo "  Eg : ./constructappimageshlos.sh --board=j7200_evm"
	echo "  list of supported boards : j721e_evm, j7200_evm, j721s2_evm, j784s4_evm"
    echo "  ./constructappimageshlos.sh --help"
    echo "       * Print usage of the script"
}

# Get user input
while [ $# -gt 0 ]; do
  case "$1" in
    --board=*)
      BOARD="${1#*=}"
      ;;
    --help)
      usage
      exit
      ;;
    *)
      printf "Error: Invalid argument $1!!\n"
      usage
      exit
  esac
  shift
done

generate_linux_appimages()
{

    # Linux-specific paths
    # --------------------

    # First, the directory where the needed Linux binaries are located needs to be defined.
    # This directory should already contain the bl31.bin, bl32.bin,
    # base-board.dtb (k3-j72*-common-proc-board.dtb), and Image files

    # Linux build directory containing all the locally-built Linux-related binaries
    LINUX_BUILD_DIR_PATH="${SBL_PATH}/tools/combined_appimage/bin/${BOARD}/"

    # Defines which appimages this script will create
    GenFiles=("atf_optee" \
            "tidtb_linux" \
            "tikernelimage_linux"
            );

    # Preserve original working directory
    pushd $PWD > /dev/null

    cd $LINUX_BUILD_DIR_PATH

    # Generate all the appimage files (as defined in GenFiles array)
    for i in "${GenFiles[@]}"
    do

        # Generate elf images
        echo "Generating $i image"
        aarch64-none-linux-gnu-ld -T $LDS_PATH/$i.lds -o $BOOTAPP_BIN_PATH/${BOARD}/$i.elf
        # Generate rprc images from elf images
        $SBL_OUT2RPRC_GEN_TOOL_PATH/out2rprc.exe $BOOTAPP_BIN_PATH/${BOARD}/$i.elf $BOOTAPP_BIN_PATH/${BOARD}/$i.rprc
        # Generate appimage from rprc image
        $MULTICORE_APPIMAGE_GEN_TOOL_PATH/MulticoreImageGen LE 55 \
            $BOOTAPP_BIN_PATH/${BOARD}/$i.appimage 0 $BOOTAPP_BIN_PATH/${BOARD}/$i.rprc
        # Remove elf and rprc images
        rm -rf $BOOTAPP_BIN_PATH/${BOARD}/$i.elf $BOOTAPP_BIN_PATH/${BOARD}/$i.rprc

    done

    # Restore shell to original working directory
    popd > /dev/null

}

sign_linux_appimages()
{
    # Signing appimages
    for i in "${GenFiles[@]}"
    do
        $PDK_INSTALL_PATH/ti/build/makerules/x509CertificateGen.sh -b $BOOTAPP_BIN_PATH/${BOARD}/$i.appimage -o $BOOTAPP_BIN_PATH/${BOARD}_hs/$i.appimage.signed -c R5 -l 0x41C00100 -k $PDK_INSTALL_PATH/ti/build/makerules/k3_dev_mpk.pem
        $PDK_INSTALL_PATH/ti/build/makerules/x509CertificateGen.sh -b $BOOTAPP_BIN_PATH/${BOARD}/$i.appimage -o $BOOTAPP_BIN_PATH/${BOARD}_hs_fs/$i.appimage.hs_fs -c R5 -l 0x41C00100 -k $PDK_INSTALL_PATH/ti/build/makerules/rom_degenerateKey.pem
    done
}

generate_linux_appimages
sign_linux_appimages