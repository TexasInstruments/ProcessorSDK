#!/bin/bash
#
# Utility script to setup SDK for SafeRTOS Builds
#
# Prerequisites:-
# - Make sure you have installed the SafeRTOS Packages from WHIS
#
# Note:-
# - Make sure the paths provided DO NOT have any spaces in them

# Example usage
# ./safertos-setup.sh --j721e_r5f_path=path/to/whis/safertos/j721e/r5f/pacakge --j721e_c66_path=path/to/whis/safertos/j721e/c66/pacakge --j721e_c7x_path=path/to/whis/safertos/j721e/c7x/pacakge
# ./safertos-setup.sh --j721e_r5f_path=path/to/whis/safertos/j721e/r5f/pacakge 
# ./safertos-setup.sh --j721e_c66_path=path/to/whis/safertos/j721e/c66/pacakge 
# ./safertos-setup.sh --j721e_c7x_path=path/to/whis/safertos/j721e/c7x/pacakge


#Environmental variables
pdk_build_path=`pwd`

#Ethernet related libraries
enet_libs="lwipstack_safertos lwipcontrib_safertos lwipport_safertos enet_example_utils_safertos enet_example_utils_full_safertos lwipif_safertos lwipific_safertos"
declare -a j721e_r5f_cores=("mcu1_0" "mcu1_1" "mcu2_0" "mcu2_1" "mcu3_0" "mcu3_1")
declare -a j7200_r5f_cores=("mcu1_0" "mcu1_1" "mcu2_0" "mcu2_1")
declare -a j721s2_r5f_cores=("mcu1_0" "mcu1_1" "mcu2_0" "mcu2_1" "mcu3_0" "mcu3_1")
declare -a j784s4_r5f_cores=("mcu1_0" "mcu1_1" "mcu2_0" "mcu2_1" "mcu3_0" "mcu3_1" "mcu4_0" "mcu4_1")

declare -a j721e_c66_cores=("c66xdsp_1" "c66xdsp_2")

declare -a j721e_c7x_cores=("c7x_1")
declare -a j721s2_c7x_cores=("c7x_1" "c7x_2")
declare -a j784s4_c7x_cores=("c7x_1" "c7x_2" "c7x_3" "c7x_4")

declare -a j721e_board_list=("j721e_evm")
declare -a j7200_board_list=("j7200_evm")
declare -a j721s2_board_list=("j721s2_evm")
declare -a j784s4_board_list=("j784s4_evm")

declare -a enet_core_list=("mcu1_0" "mcu2_0" "mcu2_1" "mcu3_0")

#Get user input
while [ $# -gt 0 ]; do
  case "$1" in
    --soc=*)
      soc="${1#*=}"
      ;;
    --isa=*)
      isa="${1#*=}"
      ;;
    --path=*)
      path="${1#*=}"
      ;;   
    -h|--help)
      echo Usage: $0 [options]
      echo
      echo Options:
      echo "--soc=<SOC>   <SOC> is in j721e j7200 j721s2 j784s4."
      echo "--isa=<ISA>   <ISA> is in r5f c7x c66."
      echo "--path=<PATH> <PATH> is the path to the WHIS SafeRTOS package for given <SOC> and <ISA>."
      exit 0
      ;;
    *)
      printf "Error: Invalid argument $1!!\n"
  esac
  shift
done

printf "\n pdk_build_path = $pdk_build_path \n"


printf "\n SOC = ${soc}\n"
printf "\n ISA = ${isa}\n"
printf "\n Path = ${path}\n\n"

if [[ ${soc} == "j7200" && ${isa} != "r5f" ]] || ([[ ${soc} == "j721s2" || ${soc} == "j784s4" ]] && [ ${isa} == "c66" ]); then
    printf "Either ISA is not present on SOC, or SafeRTOS is not supported for given ISA and SOC...!!\n\n"
    exit 1
fi

if [ ! -d ${path} ]; then
    printf "\n Error: Invalid package path provided!!\n\n"
    exit 1
fi
#Update SAFERTOS_<SOC>_<ISA>_INSTALL_PATH
sed -i -e "s|SAFERTOS_${soc}_${isa}_INSTALL_PATH =.*|SAFERTOS_${soc}_${isa}_INSTALL_PATH = ${path}|g" safertos_version.mk

ITER_CORES="${soc}_${isa}_cores[@]"
FIRST_CORE="${soc}_${isa}_cores[0]"
BOARD_LIST="${soc}_board_list[@]"
ENET_CORE_LIST="enet_core_list[@]"
for core in ${!ITER_CORES}
do
    #Build safertos lib for all R5F cores
    for board in ${!BOARD_LIST}
    do
        make safertos -sj8 CORE=$core BUILD_PROFILE=release BOARD=${board}
    done
done
#Build osal_safertos for any one core as OSAL is core independent.
make osal_safertos -sj8 CORE=${!FIRST_CORE} BUILD_PROFILE=release BOARD=${board}
if [ ${isa} == "r5f" ]; then
    for board in ${!BOARD_LIST}
    do
        for enet_core in ${!ENET_CORE_LIST}
        do
            make ${enet_libs} -sj8 CORE=${enet_core} BUILD_PROFILE=release BOARD=${board}
        done
        #Build sciserver_testapp_safertos and check-in
        make sciserver_testapp_safertos -sj8 CORE=mcu1_0 BUILD_PROFILE=release BOARD=${board}
        cp ${pdk_build_path}/../binary/sciserver_testapp_safertos/bin/${soc}/sciserver_testapp_safertos_mcu1_0_release.xer5f ${pdk_build_path}/../drv/sciclient/tools/ccsLoadDmsc/${soc}/
        cp ${pdk_build_path}/../binary/sciserver_testapp_safertos/bin/${soc}/sciserver_testapp_safertos_mcu1_0_release.rprc  ${pdk_build_path}/../drv/sciclient/tools/ccsLoadDmsc/${soc}/
    done
fi

