#!/bin/bash
#   ============================================================================
#   @file   generate_lateapps_lpm.sh
#
#   @desc   Used to generate lateapps with IPC for LPM example.
#
#   ============================================================================
#   Revision History
#   2-may-2024    Sai Ramakurthi    Initial draft
#   2-may-2024    Sparsh Kumar      Modified for LPM
#
#   ============================================================================
# Usage : Run "./generate_lateapps_lpm.sh --help" to get usage
#

PDK_INSTALL_PATH=${PWD}/../../../../..
# To get rid of ../../ in the path go to PDK_INSTALL_PATH and reassign PDK_INSTALL_PATH as PWD
pushd ${PWD}

cd ${PDK_INSTALL_PATH}
PDK_INSTALL_PATH=${PWD}

popd

PDK_BUILD_PATH=${PDK_INSTALL_PATH}/packages/ti/build
MULTICORE_APPIMAGE_GEN_TOOL_PATH=${PDK_INSTALL_PATH}/packages/ti/boot/sbl/tools/multicoreImageGen/bin
SBL_OUT2RPRC_GEN_TOOL_PATH=${PDK_INSTALL_PATH}/packages/ti/boot/sbl/tools/out2rprc/bin
BOOTAPP_BIN_PATH=${PDK_INSTALL_PATH}/packages/ti/boot/sbl/tools/BootApp_binaries

INPUT_ARGS=$@
BOARD_LIST_ALL="j721e_evm j7200_evm j721s2_evm j784s4_evm"
BOARD_LIST=

# for each of the cores, ensure that the image was properly created
ALL_CORE_IMAGES_CREATED=1

# Core lists
core_list_j721e_evm="mcu2_0 mcu2_1 mcu3_0 mcu3_1 c66xdsp_1 c66xdsp_2 c7x_1"
core_list_j7200_evm="mcu2_0 mcu2_1"
core_list_j721s2_evm="mcu2_0 mcu2_1 mcu3_0 mcu3_1 c7x_1 c7x_2"
core_list_j784s4_evm="mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3 c7x_4"

# Core extension names
declare -A core_extension=( ["mcu2_0"]="xer5f" ["mcu2_1"]="xer5f" ["mcu3_0"]="xer5f" \
                            ["mcu3_1"]="xer5f" ["mcu4_0"]="xer5f" ["mcu4_1"]="xer5f" \
                            ["c7x_1"]="xe71" ["c7x_2"]="xe71" ["c7x_3"]="xe71" \
                            ["c7x_4"]="xe71" ["c66xdsp_1"]="xe66" ["c66xdsp_2"]="xe66" )
 	
# Set Build Profile
MY_PROFILE="release"

devId=55

# Populated these appImage names in get_appimage_names() API
appImageName1=
appImageName2=

usage()
{
    echo
	echo "generate_appimage_lpm.sh => Utility script to generate lateapp images with IPC which is used in LPM Example to boot main domain cores."
	echo "Usage:"
	echo "  ./generate_lateapps_lpm.sh --all"
    echo "       * Generate appimages for all the boards"
    echo "  ./generate_appimage_lpm.sh <list_of_boards>"
    echo "       * Generate appimages for the above listed boards"
	echo "       * available board_name's: j721e_evm, j7200_evm, j721s2_evm, j784s4_evm"
    echo "  ./generate_appimage_lpm.sh --help"
    echo "       * Print usage of the script"
}

# Checks whether board_name passed as argument to this api is present in BOARD_LIST_ALL or not.
exists_in_board_list() 
{
    return_val=0
    for item in $BOARD_LIST_ALL
    do
        if [ $1 == $item ]
        then
            return_val=1
            return
        fi
    done
}

get_appimage_names()
{
    if [ "$1" = "j721e_evm" ]
    then
        appImageName1=multicore_LPM_MCU2_0_MCU2_1_stage1.appimage
        appImageName2=multicore_LPM_DSPs_MCU3_0_MCU3_1_stage2.appimage
    elif [ "$1" = "j7200_evm" ]
    then
        appImageName1=multicore_LPM_MCU2_0_MCU2_1_stage1.appimage
    elif [ "$1" = "j721s2_evm" ]
    then
        appImageName1=multicore_LPM_MCU2_0_MCU2_1_stage1.appimage
        appImageName2=multicore_LPM_DSP_1_2_C7x_MCU3_0_MCU3_1_stage2.appimage
    elif [ "$1" = "j784s4_evm" ]
    then
        appImageName1=multicore_LPM_MCU2_0_MCU2_1_stage1.appimage
        appImageName2=multicore_LPM_DSPs_MCU3_0_MCU3_1_MCU4_0_MCU4_1_stage2.appimage
    fi
}

# Populate BOARD_LIST variable
set_board_list()
{
    if [ ${#INPUT_ARGS[@]} -le "1" ]
    then
        exists_in_board_list $INPUT_ARGS 
        if [ $INPUT_ARGS == "--all" ]
        then
            BOARD_LIST=$BOARD_LIST_ALL
        elif [ $INPUT_ARGS == "--help" ]
        then
            usage
            exit 0
        elif [ $return_val == "1" ]
        then
            BOARD_LIST=$INPUT_ARGS
        else
            echo "Invalid Arguments !!"
            usage
        fi
    else
        for board in $INPUT_ARGS
        do
            exists_in_board_list $board
            if [ $return_val == "1" ]
            then
                BOARD_LIST+=" $board"
            fi
        done
    fi
}

# Build bootapp_boot_test
build_bootapp_boot_test()
{
    pushd ${PWD}
    cd ${PDK_BUILD_PATH}

    for BOARD_NAME in ${BOARD_LIST}
    do
        core_list=core_list_${BOARD_NAME}
        for CORE_NAME in ${!core_list}
        do
            echo "Building ipc_echo_test_freertos for board : $BOARD_NAME, core : ${CORE_NAME}"            
                make BOARD=${BOARD_NAME} CORE=${CORE_NAME} ipc_echo_test_freertos BUILD_PROFILE=${MY_PROFILE} -sj8
                FILE=${PDK_INSTALL_PATH}/packages/ti/binary/ipc_echo_test_freertos/bin/${BOARD_NAME%_*}_evm/ipc_echo_test_freertos_${CORE_NAME}_${MY_PROFILE}.${core_extension[${CORE_NAME}]}
            if [ ! -f "$FILE" ]
            then
                echo "Error - $FILE does not exist"
                ALL_CORE_IMAGES_CREATED=0
                break
            fi
        done
    done
    popd
}

generate_lateapps()
{
    if [ $ALL_CORE_IMAGES_CREATED == 1 ]
    then
        echo "Executables of all the cores have been created"

        # Get executable list
        for BOARD_NAME in ${BOARD_LIST}
        do
            if [ $BOARD_NAME == "j721e_evm" ]
            then
                # elf images list per stage
                elf_images_list_stage1_j721e_evm=("ipc_echo_test_freertos_mcu2_0_release.xer5f" \
                                                  "ipc_echo_test_freertos_mcu2_1_release.xer5f" )
                elf_images_list_stage2_j721e_evm=("ipc_echo_test_freertos_mcu3_0_release.xer5f" \
                                                "ipc_echo_test_freertos_mcu3_1_release.xer5f" \
                                                "ipc_echo_test_freertos_c66xdsp_1_release.xe66" \
                                                "ipc_echo_test_freertos_c66xdsp_2_release.xe66" \
                                                "ipc_echo_test_freertos_c7x_1_release.xe71")

                # RPRC files list per stage
                rprc_file_list_stage1_j721e_evm=("ipc_echo_test_freertos_mcu2_0_release.xer5f.rprc" \
                                                "ipc_echo_test_freertos_mcu2_1_release.xer5f.rprc")
                rprc_file_list_stage2_j721e_evm=("ipc_echo_test_freertos_mcu3_0_release.xer5f.rprc" \
                                                "ipc_echo_test_freertos_mcu3_1_release.xer5f.rprc" \
                                                "ipc_echo_test_freertos_c66xdsp_1_release.xe66.rprc" \
                                                "ipc_echo_test_freertos_c66xdsp_2_release.xe66.rprc" \
                                                "ipc_echo_test_freertos_c7x_1_release.xe71.rprc")

                core_ids_stage1_j721e_evm=(10 11)
                core_ids_stage2_j721e_evm=(12 13 16 17 18)

            elif [ $BOARD_NAME == "j7200_evm" ]
            then
                # elf images list per stage
                elf_images_list_stage1_j7200_evm=("ipc_echo_test_freertos_mcu2_0_release.xer5f" \
                                                  "ipc_echo_test_freertos_mcu2_1_release.xer5f" )

                # RPRC files list per stage
                rprc_file_list_stage1_j7200_evm=("ipc_echo_test_freertos_mcu2_0_release.xer5f.rprc" \
                                                "ipc_echo_test_freertos_mcu2_1_release.xer5f.rprc")

                core_ids_stage1_j7200_evm=(10 11)

            elif [ $BOARD_NAME == "j721s2_evm" ]
            then
                # elf images list per stage
                elf_images_list_stage1_j721s2_evm=("ipc_echo_test_freertos_mcu2_0_release.xer5f" \
                                                  "ipc_echo_test_freertos_mcu2_1_release.xer5f" )
                elf_images_list_stage2_j721s2_evm=("ipc_echo_test_freertos_mcu3_0_release.xer5f" \
                                                "ipc_echo_test_freertos_mcu3_1_release.xer5f" \
                                                "ipc_echo_test_freertos_c7x_1_release.xe71" \
                                                "ipc_echo_test_freertos_c7x_2_release.xe71")

                # RPRC files list per stage
                rprc_file_list_stage1_j721s2_evm=("ipc_echo_test_freertos_mcu2_0_release.xer5f.rprc" \
                                                "ipc_echo_test_freertos_mcu2_1_release.xer5f.rprc")
                rprc_file_list_stage2_j721s2_evm=("ipc_echo_test_freertos_mcu3_0_release.xer5f.rprc" \
                                                "ipc_echo_test_freertos_mcu3_1_release.xer5f.rprc" \
                                                "ipc_echo_test_freertos_c7x_1_release.xe71.rprc" \
                                                "ipc_echo_test_freertos_c7x_2_release.xe71.rprc")

                core_ids_stage1_j721s2_evm=(10 11)
                core_ids_stage2_j721s2_evm=(12 13 18 19)

            elif [ $BOARD_NAME == "j784s4_evm" ]
            then
                # elf images list per stage
                elf_images_list_stage1_j784s4_evm=("ipc_echo_test_freertos_mcu2_0_release.xer5f" \
                                                  "ipc_echo_test_freertos_mcu2_1_release.xer5f" )
                elf_images_list_stage2_j784s4_evm=("ipc_echo_test_freertos_mcu3_0_release.xer5f" \
                                                "ipc_echo_test_freertos_mcu3_1_release.xer5f" \
                                                "ipc_echo_test_freertos_mcu4_0_release.xer5f" \
                                                "ipc_echo_test_freertos_mcu4_1_release.xer5f" \
                                                "ipc_echo_test_freertos_c7x_1_release.xe71" \
                                                "ipc_echo_test_freertos_c7x_2_release.xe71" \
                                                "ipc_echo_test_freertos_c7x_3_release.xe71" \
                                                "ipc_echo_test_freertos_c7x_4_release.xe71")

                # RPRC files list per stage
                rprc_file_list_stage1_j784s4_evm=("ipc_echo_test_freertos_mcu2_0_release.xer5f.rprc" \
                                                "ipc_echo_test_freertos_mcu2_1_release.xer5f.rprc")
                rprc_file_list_stage2_j784s4_evm=("ipc_echo_test_freertos_mcu3_0_release.xer5f.rprc" \
                                                "ipc_echo_test_freertos_mcu3_1_release.xer5f.rprc" \
                                                "ipc_echo_test_freertos_mcu4_0_release.xer5f.rprc" \
                                                "ipc_echo_test_freertos_mcu4_1_release.xer5f.rprc" \
                                                "ipc_echo_test_freertos_c7x_1_release.xe71.rprc" \
                                                "ipc_echo_test_freertos_c7x_2_release.xe71.rprc" \
                                                "ipc_echo_test_freertos_c7x_3_release.xe71.rprc" \
                                                "ipc_echo_test_freertos_c7x_4_release.xe71.rprc")

                core_ids_stage1_j784s4_evm=(10 11)
                core_ids_stage2_j784s4_evm=(12 13 14 15 18 19 20 21)
            fi
        done
        
        for BOARD_NAME in ${BOARD_LIST}
        do
            # Generate output arguments for Stage 1 appimage
            output_args1=()
            core_ids_list=core_ids_stage1_${BOARD_NAME}[@]
            core_ids_list=(${!core_ids_list})
            rprc_file_list=rprc_file_list_stage1_${BOARD_NAME}[@]
            rprc_file_list=(${!rprc_file_list})
            arrayLength=${#core_ids_list[@]}
            echo "Stage 1 length is $arrayLength"
            for (( i = 0; i < $arrayLength; i++ )); do
                output_args1+="${core_ids_list[i]} ${rprc_file_list[i]} "
            done

            if [ ${BOARD_NAME} != "j7200_evm" ]
            then
                # Generate output arguments for Stage 2 appimage
                output_args2=()
                core_ids_list=core_ids_stage2_${BOARD_NAME}[@]
                core_ids_list=(${!core_ids_list})
                rprc_file_list=rprc_file_list_stage2_${BOARD_NAME}[@]
                rprc_file_list=(${!rprc_file_list})
                arrayLength=${#core_ids_list[@]}
                echo "Stage 2 length is $arrayLength"
                for (( i = 0; i < $arrayLength; i++ )); do
                    output_args2+="${core_ids_list[i]} ${rprc_file_list[i]} "
                done
            fi
            
            rm -rf ${PDK_INSTALL_PATH}/packages/ti/drv/lpm/lateapps/${BOARD_NAME}/*
            mkdir -p ${PDK_INSTALL_PATH}/packages/ti/drv/lpm/lateapps/${BOARD_NAME}
            MULTICOREAPP_BIN_PATH=${PDK_INSTALL_PATH}/packages/ti/drv/lpm/lateapps/${BOARD_NAME}
            pushd ${PWD}
            cd ${MULTICOREAPP_BIN_PATH}

            # Generate stage1 rprc files
            elf_images_list=elf_images_list_stage1_${BOARD_NAME}[@]
            elf_images_list=(${!elf_images_list})
            for i in ${elf_images_list[@]}; do
                # Use mono execute windows application in linux machine
                mono $SBL_OUT2RPRC_GEN_TOOL_PATH/out2rprc.exe ${PDK_INSTALL_PATH}/packages/ti/binary/ipc_echo_test_freertos/bin/${BOARD_NAME}/$i "$i.rprc"
            done

            # Generate stage2 rprc files
            if [ $BOARD_NAME != "j7200_evm" ]
            then
                elf_images_list=elf_images_list_stage2_${BOARD_NAME}[@]
                elf_images_list=(${!elf_images_list})
                for i in ${elf_images_list[@]}; do
                    # Use mono execute windows application in linux machine
                    mono $SBL_OUT2RPRC_GEN_TOOL_PATH/out2rprc.exe ${PDK_INSTALL_PATH}/packages/ti/binary/ipc_echo_test_freertos/bin/${BOARD_NAME}/$i "$i.rprc"
                done
            fi

            get_appimage_names "${BOARD_NAME}"
            # Generate and sign lateapp1
            $MULTICORE_APPIMAGE_GEN_TOOL_PATH/MulticoreImageGen LE $devId $appImageName1 $output_args1
            ${PDK_INSTALL_PATH}/packages/ti/build/makerules/x509CertificateGen.sh -b ${PDK_INSTALL_PATH}/packages/ti/drv/lpm/lateapps/${BOARD_NAME}/$appImageName1 -o $MULTICOREAPP_BIN_PATH/$appImageName1.signed -c R5 -l 0x41C00100 -k ${PDK_INSTALL_PATH}/packages/ti/build/makerules/k3_dev_mpk.pem
            ${PDK_INSTALL_PATH}/packages/ti/build/makerules/x509CertificateGen.sh -b ${PDK_INSTALL_PATH}/packages/ti/drv/lpm/lateapps/${BOARD_NAME}/$appImageName1 -o $MULTICOREAPP_BIN_PATH/$appImageName1.hs_fs -c R5 -l 0x41C00100 -k ${PDK_INSTALL_PATH}/packages/ti/build/makerules/rom_degenerateKey.pem
            # Generate and sign lateapp2
            if [ $BOARD_NAME != "j7200_evm" ]
            then
                $MULTICORE_APPIMAGE_GEN_TOOL_PATH/MulticoreImageGen LE $devId $appImageName2 $output_args2
                ${PDK_INSTALL_PATH}/packages/ti/build/makerules/x509CertificateGen.sh -b ${PDK_INSTALL_PATH}/packages/ti/drv/lpm/lateapps/${BOARD_NAME}/$appImageName2 -o $MULTICOREAPP_BIN_PATH/$appImageName2.signed -c R5 -l 0x41C00100 -k ${PDK_INSTALL_PATH}/packages/ti/build/makerules/k3_dev_mpk.pem
                ${PDK_INSTALL_PATH}/packages/ti/build/makerules/x509CertificateGen.sh -b ${PDK_INSTALL_PATH}/packages/ti/drv/lpm/lateapps/${BOARD_NAME}/$appImageName2 -o $MULTICOREAPP_BIN_PATH/$appImageName2.hs_fs -c R5 -l 0x41C00100 -k ${PDK_INSTALL_PATH}/packages/ti/build/makerules/rom_degenerateKey.pem
            fi
            popd
        done

    fi
}

set_board_list
build_bootapp_boot_test
generate_lateapps
