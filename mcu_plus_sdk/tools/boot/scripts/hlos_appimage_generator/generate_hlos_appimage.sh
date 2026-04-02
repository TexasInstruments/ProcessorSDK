#!/bin/bash
#   ============================================================================
#   @file   generate_hlos_appimage.sh
#
#   @desc
#
#   ============================================================================
#   Revision History
#   27-Feb-2024  Sai Ramakurthi        Initial draft
#
#   ============================================================================
# Usage : Run "./generate_appimage_hlos.sh --help" to get usage
# This script needs to run for every release with --release_version=<latest version>
#

MCUPLUS_SDK_INSTALL_PATH=${PWD}/../../../..

# To get rid of ../../ in the path go to MCUPLUS_SDK_INSTALL_PATH and reassign MCUPLUS_SDK_INSTALL_PATH as PWD
pushd ${PWD}
cd ${MCUPLUS_SDK_INSTALL_PATH}
MCUPLUS_SDK_INSTALL_PATH=${PWD}
popd

HLOS_PREBUILT_BIN_PATH=${MCUPLUS_SDK_INSTALL_PATH}/tools/boot/hlos_prebuilt
BOARD_LIST=

usage()
{
    echo
	echo "generate_appimage_hlos.sh => Utility script to generate appimages to boot Linux, U-Boot, QNX from SBL"
	echo "Usage:"
	echo "  ./generate_appimage_hlos.sh --board_list=<list of boards> --release_version=<any release version or latest> --jacinto_rtos_test_repo_path=<path to the jacinto_rtos_test repository>"
    echo "  Eg : ./generate_appimage_hlos.sh --board_list=\"j722s-evm\" --release_version=latest"
	echo "  list of supported boards : j722s-evm"
    echo "  ./generate_appimage_hlos.sh --help"
    echo "       * Print usage of the script"
}

# Set Proxies
set_proxies() {
    export HTTPS_PROXY=http://webproxy.ext.ti.com:80
    export https_proxy=http://webproxy.ext.ti.com:80
    export HTTP_PROXY=http://webproxy.ext.ti.com:80
    export http_proxy=http://webproxy.ext.ti.com:80
    export ftp_proxy=http://webproxy.ext.ti.com:80
    export FTP_PROXY=http://webproxy.ext.ti.com:80
    export no_proxy=ti.com
}

#Get user input
while [ $# -gt 0 ]; do
  case "$1" in
    --board_list=*)
      BOARD_LIST="${1#*=}"
      ;;
    --release_version=*)
      LINUX_RELEASE_VERSION="${1#*=}"
      ;;
    --jacinto_rtos_test_repo_path=*)
      JACINTO_RTOS_TEST_REPO_PATH="${1#*=}"
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

: ${LINUX_RELEASE_VERSION:="latest"}

# Download images from lcpd nightly build which are needed to generate hlos appimages
download_linux_images()
{
    for BOARD_NAME in $BOARD_LIST
    do
        HS_SUFFIX=
        mkdir temp
        pushd ${PWD}
        cd temp

        # Download bl31.bin
        wget http://lcpd.itg.ti.com/builds/arago-scarthgap/prod/${BOARD_NAME}/${LINUX_RELEASE_VERSION}/images/bl31.bin
        mv bl31.bin ${HLOS_PREBUILT_BIN_PATH}/${BOARD_NAME}/linux

        # Download bl32.bin
        wget http://lcpd.itg.ti.com/builds/arago-scarthgap/prod/${BOARD_NAME}/${LINUX_RELEASE_VERSION}/images/bl32.bin
        mv bl32.bin ${HLOS_PREBUILT_BIN_PATH}/${BOARD_NAME}/linux

        # Download Image
        wget http://lcpd.itg.ti.com/builds/arago-scarthgap/prod/${BOARD_NAME}/${LINUX_RELEASE_VERSION}/images/Image
        mv Image ${HLOS_PREBUILT_BIN_PATH}/${BOARD_NAME}/linux

        # Download u-boot
        wget http://lcpd.itg.ti.com/builds/arago-scarthgap/prod/${BOARD_NAME}/${LINUX_RELEASE_VERSION}/images/u-boot.img
        mv u-boot.img ${HLOS_PREBUILT_BIN_PATH}/${BOARD_NAME}/linux/u-boot.img

        # Download u-boot-spl.bin
        wget http://lcpd.itg.ti.com/builds/arago-scarthgap/prod/${BOARD_NAME}/${LINUX_RELEASE_VERSION}/images/u-boot-spl.bin
        mv u-boot-spl.bin ${HLOS_PREBUILT_BIN_PATH}/${BOARD_NAME}/linux/u-boot-spl-${BOARD_NAME}.bin

        popd

        rm -rf temp
    done
}

download_qnx_images()
{
    pushd ${PWD}
    mkdir temp_folder
    cd temp_folder

    wget https://lcpd.itg.ti.com/jacinto/PSDK_QNX/DOCKER_BUILD/latest/J722S/prod/artifacts/output/J722S_0/bootfs_spl_QNX800_j722s_evm.tar.gz --no-check-certificate

    mkdir qnx_images
    mkdir -p ${HLOS_PREBUILT_BIN_PATH}/${BOARD_NAME}/qnx
    tar -xvzf bootfs_spl_QNX800_j722s_evm.tar.gz -C qnx_images
    rm -f bootfs_spl_QNX800_j722s_evm.tar.gz
    cp qnx_images/qnx-ifs ${HLOS_PREBUILT_BIN_PATH}/${BOARD_NAME}/qnx
    popd

    rm -rf temp_folder
}

build_hlos_appimage()
{
    pushd ${PWD}

    cd ${MCUPLUS_SDK_INSTALL_PATH}/tools/boot/linuxAppimageGen

    make BOARD=j722s-evm -sj8 clean
    # Build linux appimage
    make BOARD=j722s-evm FASTBOOT_LINUX=1 -sj8
    cp board/${BOARD_NAME}/linux.appimage.hs_fs ${HLOS_PREBUILT_BIN_PATH}/${BOARD_NAME}/linux/linux_opt.appimage.hs_fs

    make BOARD=j722s-evm -sj8 clean
    # Build U-Boot appimage
    make BOARD=j722s-evm -sj8
    cp board/${BOARD_NAME}/linux.appimage.hs_fs ${HLOS_PREBUILT_BIN_PATH}/${BOARD_NAME}/linux/linux_dev.appimage.hs_fs

    cd ${MCUPLUS_SDK_INSTALL_PATH}/tools/boot/qnxAppimageGen
    make BOARD=j722s-evm -sj8 clean
    # Build and Copy QNX appimage to jacinto rtos test repo
    make BOARD=j722s-evm -sj8
    cp board/${BOARD_NAME}/qnx.appimage.hs_fs  $JACINTO_RTOS_TEST_REPO_PATH/qnx_binaries/j722s_evm_hs_fs
    cp ${HLOS_PREBUILT_BIN_PATH}/${BOARD_NAME}/qnx/qnx-ifs  $JACINTO_RTOS_TEST_REPO_PATH/qnx_binaries/j722s_evm_hs_fs

    # Remove qnx specific folder
    rm -rf ${HLOS_PREBUILT_BIN_PATH}/${BOARD_NAME}/qnx

    popd

}

set_proxies
download_linux_images
download_qnx_images
build_hlos_appimage
