#!/bin/bash
#   ============================================================================
#   @file   ipc_firmware_release.sh
#
#   @desc   script to build copy ipc linux firmware
#
#   ============================================================================
#   Revision History
#   28-oct-2024  Sai Ramakurthi        Initial draft
#
#   ============================================================================
# Usage : Run "./ipc_firmware_release.sh --help" to get usage
# This script needs to run for every release with --release_version=<latest version>
#

PDK_INSTALL_PATH=${PWD}/../../../../

set -x

# To get rid of ../../ in the path go to PDK_INSTALL_PATH and reassign PDK_INSTALL_PATH as PWD
pushd ${PWD}

cd ${PDK_INSTALL_PATH}
PDK_INSTALL_PATH=${PWD}

popd

PDK_BUILD_PATH=${PDK_INSTALL_PATH}/ti/build
SOC_LIST_ALL="j721e j7200 j721s2 j784s4 j742s2"
SOC_LIST=
CORE_LIST_j721e_evm="mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c66xdsp_1 c66xdsp_2 c7x_1"
CORE_LIST_j7200_evm="mcu1_1 mcu2_0 mcu2_1"
CORE_LIST_j721s2_evm="mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c7x_1 c7x_2"
CORE_LIST_j784s4_evm="mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3 c7x_4"
CORE_LIST_j742s2_evm="mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3"

declare -A elf_extensions
elf_extensions["mcu1_0"]="xer5f"
elf_extensions["mcu1_1"]="xer5f"
elf_extensions["mcu2_0"]="xer5f"
elf_extensions["mcu2_1"]="xer5f"
elf_extensions["mcu3_0"]="xer5f"
elf_extensions["mcu3_1"]="xer5f"
elf_extensions["mcu4_0"]="xer5f"
elf_extensions["mcu4_1"]="xer5f"
elf_extensions["c7x_1"]="xe71"
elf_extensions["c7x_2"]="xe71"
elf_extensions["c7x_3"]="xe71"
elf_extensions["c7x_4"]="xe71"
elf_extensions["c66xdsp_1"]="xe66"
elf_extensions["c66xdsp_2"]="xe66"

echo ${elf_extensions[mcu1_0]}

usage()
{
	echo "This script is used to generate images needed for ipc firmware release"
  echo "Usage :"
  echo "./ipc_firmware_release.sh --soc_list=<list of soc's> --ti_linux_firmware_repo_path=<path to linux firmware repo>"
  echo "Eg: "
  echo "./ipc_firmware_release.sh --soc_list=\"j7200 j721e\" --ti_linux_firmware_repo_path=/home/sai/ti/ti-linux-firmware"
  echo "for all socs, you can use --soc_list=all"
}

#Get user input
while [ $# -gt 0 ]; do
  case "$1" in
    --soc_list=*)
      SOC_LIST="${1#*=}"
      ;;
    --all*)
      SOC_LIST=$SOC_LIST_ALL
      ;;
    --ti_linux_firmware_repo_path=*)
      TI_LINUX_FIRMWARE_REPO_PATH="${1#*=}"
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

build_ipc_firmware()
{
    pushd ${PWD}

    cd ${PDK_BUILD_PATH}
    for soc in $SOC_LIST
    do
        echo "Soc : $soc "
        corelist="CORE_LIST_${soc}_evm"
        for core in ${!corelist}
        do
            echo "Building ipc_echo_test_freertos for Board : ${soc}, Core : ${core} ....."
            make -sj8 BOARD=${soc}_evm CORE=${core} ipc_echo_test_freertos
            echo "done"
        done
    done

    popd
}

build_ipc_rtos_echo_test_freertos()
{
    pushd ${PWD}

    cd ${PDK_BUILD_PATH}
    for soc in $SOC_LIST
    do
        # Build for mcu1_0 as well
        make -sj8 BOARD=${soc}_evm CORE=mcu1_0 ipc_rtos_echo_test_freertos

        corelist="CORE_LIST_${soc}_evm"
        for core in ${!corelist}
        do
            echo "Building ipc_rtos_echo_test_freertos for Board : ${soc}_evm, Core : ${core} ....."
            make -sj8 BOARD=${soc}_evm CORE=${core} ipc_rtos_echo_test_freertos
            echo "done"
            last_core=$core
        done
        # Build multicore test as well to validate
        make -sj8 BOARD=${soc}_evm CORE=${last_core} ipc_rtos_multicore_echo_test_freertos
    done

    popd
}

copy_linux_firmware()
{
  for soc in $SOC_LIST
  do
      corelist="CORE_LIST_${soc}_evm"
      for core in ${!corelist}
      do
        echo "---> $TI_LINUX_FIRMWARE_REPO_PATH/ti-ipc/${soc}/ipc_echo_test_${core}_release_strip.${elf_extensions[$core]}"
        cp $PDK_INSTALL_PATH/ti/binary/ipc_echo_test_freertos/bin/${soc}_evm/ipc_echo_test_freertos_${core}_release_strip.${elf_extensions[$core]} $TI_LINUX_FIRMWARE_REPO_PATH/ti-ipc/${soc}/ipc_echo_test_${core}_release_strip.${elf_extensions[$core]}
      done
  done
}

find_md5checksum()
{
  pushd ${PWD}

  cd $TI_LINUX_FIRMWARE_REPO_PATH
  for soc in $SOC_LIST
  do
      rm -f ${soc}_md5sum.txt
      touch ${soc}_md5sum.txt
      corelist="CORE_LIST_${soc}_evm"
      for core in ${!corelist}
      do
        md5sum ti-ipc/${soc}/ipc_echo_test_${core}_release_strip.${elf_extensions[$core]} >> ${soc}_md5sum.txt
      done
  done

  popd
}

build_ipc_firmware
build_ipc_rtos_echo_test_freertos
copy_linux_firmware
find_md5checksum
