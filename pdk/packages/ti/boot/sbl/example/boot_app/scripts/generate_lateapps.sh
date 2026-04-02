#!/bin/bash
#   ============================================================================
#   @file   generate_lateapps.sh
#
#   @desc   
#
#   ============================================================================
#   Revision History
#   1-april-2023  Sai Ramakurthi        Initial draft
#
#   ============================================================================
# Usage : Run "./generate_lateapps.sh --help" to get usage
#

PDK_INSTALL_PATH=${PWD}/../../../../../../..

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
BOARD_LIST_ALL="j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm"
BOARD_LIST=

# for each of the cores, ensure that the image was properly created
ALL_CORE_IMAGES_CREATED=1

# Core lists
core_list_j721e_evm="mpu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c66xdsp_1 c66xdsp_2 c7x_1"
core_list_j7200_evm="mpu1_0 mcu2_0 mcu2_1"
core_list_j721s2_evm="mpu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 c7x_1 c7x_2"
core_list_j784s4_evm="mpu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3 c7x_4"
core_list_j742s2_evm="mpu1_0 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1 c7x_1 c7x_2 c7x_3"

# Core extension names
declare -A core_extension=( ["mcu2_0"]="xer5f" ["mcu2_1"]="xer5f" ["mcu3_0"]="xer5f" \
                            ["mcu3_1"]="xer5f" ["mcu4_0"]="xer5f" ["mcu4_1"]="xer5f" \
                            ["mpu1_0"]="xa72fg" ["c7x_1"]="xe71" ["c7x_2"]="xe71" \
                            ["c7x_3"]="xe71" ["c7x_4"]="xe71" ["c66xdsp_1"]="xe66" \
                            ["c66xdsp_2"]="xe66" )

# Set Build Profile
MY_PROFILE="release"

devId=55

# Populated these appImage names in get_appimage_names() API
appImageName1=
appImageName2=
appImageName3=

usage()
{
    echo
	echo "generate_appimage_hlos.sh => Utility script to generate lateapp images which boots main domain cores"
	echo "Usage:"
	echo "  ./generate_lateapps.sh --all"
    echo "       * Generate appimages for all the boards"
    echo "  ./generate_appimage_hlos.sh <list_of_boards>"
    echo "       * Generate appimages for the above listed boards"
	echo "       * available board_name's: j721e_evm, j7200_evm, j721s2_evm, j784s4_evm, j742s2_evm"
    echo "  ./generate_appimage_hlos.sh --help"
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
        appImageName1=multicore_MCU2_0_MCU2_1_stage1.appimage
        appImageName2=multicore_DSPs_MCU3_0_MCU3_1_stage2.appimage
        appImageName3=multicore_MPU1_0_stage3.appimage
    elif [ "$1" = "j7200_evm" ]
    then
        appImageName1=multicore_MCU2_0_MCU2_1_stage1.appimage
        appImageName2=multicore_MPU1_0_stage2.appimage
    elif [ "$1" = "j721s2_evm" ]
    then
        appImageName1=multicore_MCU2_0_MCU2_1_stage1.appimage
        appImageName2=multicore_DSP_1_2_C7x_MCU3_0_MCU3_1_stage2.appimage
        appImageName3=multicore_MPU1_0_stage3.appimage
    elif [ "$1" = "j784s4_evm" ] || [ "$1" = "j742s2_evm" ]
    then
        appImageName1=multicore_MCU2_0_MCU2_1_stage1.appimage
        appImageName2=multicore_DSPs_MCU3_0_MCU3_1_MCU4_0_MCU4_1_stage2.appimage
        appImageName3=multicore_MPU1_0_stage3.appimage
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
            echo "Building bootapp_boot_test for board : $BOARD_NAME, core : ${CORE_NAME}"
            make BOARD=${BOARD_NAME} CORE=${CORE_NAME} bootapp_boot_test BUILD_PROFILE=${MY_PROFILE} -s -j
            FILE=${PDK_INSTALL_PATH}/packages/ti/boot/sbl/example/k3MulticoreApp/binary/${BOARD_NAME%_*}/bootapp_boot_test_${BOARD_NAME}_${CORE_NAME}TestApp_${MY_PROFILE}.${core_extension[${CORE_NAME}]}
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
                elf_images_list_stage1_j721e_evm=("bootapp_boot_test_j721e_evm_mcu2_0TestApp_release.xer5f" \
                                                  "bootapp_boot_test_j721e_evm_mcu2_1TestApp_release.xer5f" )
                elf_images_list_stage2_j721e_evm=("bootapp_boot_test_j721e_evm_mcu3_0TestApp_release.xer5f" \
                                                "bootapp_boot_test_j721e_evm_mcu3_1TestApp_release.xer5f" \
                                                "bootapp_boot_test_j721e_evm_c66xdsp_1TestApp_release.xe66" \
                                                "bootapp_boot_test_j721e_evm_c66xdsp_2TestApp_release.xe66" \
                                                "bootapp_boot_test_j721e_evm_c7x_1TestApp_release.xe71")
                elf_images_list_stage3_j721e_evm=("bootapp_boot_test_j721e_evm_mpu1_0TestApp_release.xa72fg")

                # RPRC files list per stage
                rprc_file_list_stage1_j721e_evm=("bootapp_boot_test_j721e_evm_mcu2_0TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j721e_evm_mcu2_1TestApp_release.xer5f.rprc")
                rprc_file_list_stage2_j721e_evm=("bootapp_boot_test_j721e_evm_mcu3_0TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j721e_evm_mcu3_1TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j721e_evm_c66xdsp_1TestApp_release.xe66.rprc" \
                                                "bootapp_boot_test_j721e_evm_c66xdsp_2TestApp_release.xe66.rprc" \
                                                "bootapp_boot_test_j721e_evm_c7x_1TestApp_release.xe71.rprc")
                rprc_file_list_stage3_j721e_evm=("bootapp_boot_test_j721e_evm_mpu1_0TestApp_release.xa72fg.rprc")

                core_ids_stage1_j721e_evm=(10 11)
                core_ids_stage2_j721e_evm=(12 13 16 17 18)
                core_ids_stage3_j721e_evm=(0)
            elif [ $BOARD_NAME == "j7200_evm" ]
            then
                # elf images list per stage
                elf_images_list_stage1_j7200_evm=("bootapp_boot_test_j7200_evm_mcu2_0TestApp_release.xer5f" \
                                                  "bootapp_boot_test_j7200_evm_mcu2_1TestApp_release.xer5f" )
                elf_images_list_stage2_j7200_evm=("bootapp_boot_test_j7200_evm_mpu1_0TestApp_release.xa72fg")

                # RPRC files list per stage
                rprc_file_list_stage1_j7200_evm=("bootapp_boot_test_j7200_evm_mcu2_0TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j7200_evm_mcu2_1TestApp_release.xer5f.rprc")
                rprc_file_list_stage2_j7200_evm=("bootapp_boot_test_j7200_evm_mpu1_0TestApp_release.xa72fg.rprc")

                core_ids_stage1_j7200_evm=(10 11)
                core_ids_stage2_j7200_evm=(0)
            elif [ $BOARD_NAME == "j721s2_evm" ]
            then
                # elf images list per stage
                elf_images_list_stage1_j721s2_evm=("bootapp_boot_test_j721s2_evm_mcu2_0TestApp_release.xer5f" \
                                                  "bootapp_boot_test_j721s2_evm_mcu2_1TestApp_release.xer5f" )
                elf_images_list_stage2_j721s2_evm=("bootapp_boot_test_j721s2_evm_mcu3_0TestApp_release.xer5f" \
                                                "bootapp_boot_test_j721s2_evm_mcu3_1TestApp_release.xer5f" \
                                                "bootapp_boot_test_j721s2_evm_c7x_1TestApp_release.xe71" \
                                                "bootapp_boot_test_j721s2_evm_c7x_2TestApp_release.xe71")
                elf_images_list_stage3_j721s2_evm=("bootapp_boot_test_j721s2_evm_mpu1_0TestApp_release.xa72fg")

                # RPRC files list per stage
                rprc_file_list_stage1_j721s2_evm=("bootapp_boot_test_j721s2_evm_mcu2_0TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j721s2_evm_mcu2_1TestApp_release.xer5f.rprc")
                rprc_file_list_stage2_j721s2_evm=("bootapp_boot_test_j721s2_evm_mcu3_0TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j721s2_evm_mcu3_1TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j721s2_evm_c7x_1TestApp_release.xe71.rprc" \
                                                "bootapp_boot_test_j721s2_evm_c7x_2TestApp_release.xe71.rprc")
                rprc_file_list_stage3_j721s2_evm=("bootapp_boot_test_j721s2_evm_mpu1_0TestApp_release.xa72fg.rprc")

                core_ids_stage1_j721s2_evm=(10 11)
                core_ids_stage2_j721s2_evm=(12 13 18 19)
                core_ids_stage3_j721s2_evm=(0)
            elif [ $BOARD_NAME == "j784s4_evm" ]
            then
                # elf images list per stage
                elf_images_list_stage1_j784s4_evm=("bootapp_boot_test_j784s4_evm_mcu2_0TestApp_release.xer5f" \
                                                  "bootapp_boot_test_j784s4_evm_mcu2_1TestApp_release.xer5f" )
                elf_images_list_stage2_j784s4_evm=("bootapp_boot_test_j784s4_evm_mcu3_0TestApp_release.xer5f" \
                                                "bootapp_boot_test_j784s4_evm_mcu3_1TestApp_release.xer5f" \
                                                "bootapp_boot_test_j784s4_evm_mcu4_0TestApp_release.xer5f" \
                                                "bootapp_boot_test_j784s4_evm_mcu4_1TestApp_release.xer5f" \
                                                "bootapp_boot_test_j784s4_evm_c7x_1TestApp_release.xe71" \
                                                "bootapp_boot_test_j784s4_evm_c7x_2TestApp_release.xe71" \
                                                "bootapp_boot_test_j784s4_evm_c7x_3TestApp_release.xe71" \
                                                "bootapp_boot_test_j784s4_evm_c7x_4TestApp_release.xe71")
                elf_images_list_stage3_j784s4_evm=("bootapp_boot_test_j784s4_evm_mpu1_0TestApp_release.xa72fg")

                # RPRC files list per stage
                rprc_file_list_stage1_j784s4_evm=("bootapp_boot_test_j784s4_evm_mcu2_0TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j784s4_evm_mcu2_1TestApp_release.xer5f.rprc")
                rprc_file_list_stage2_j784s4_evm=("bootapp_boot_test_j784s4_evm_mcu3_0TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j784s4_evm_mcu3_1TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j784s4_evm_mcu4_0TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j784s4_evm_mcu4_1TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j784s4_evm_c7x_1TestApp_release.xe71.rprc" \
                                                "bootapp_boot_test_j784s4_evm_c7x_2TestApp_release.xe71.rprc" \
                                                "bootapp_boot_test_j784s4_evm_c7x_3TestApp_release.xe71.rprc" \
                                                "bootapp_boot_test_j784s4_evm_c7x_4TestApp_release.xe71.rprc")
                rprc_file_list_stage3_j784s4_evm=("bootapp_boot_test_j784s4_evm_mpu1_0TestApp_release.xa72fg.rprc")

                core_ids_stage1_j784s4_evm=(10 11)
                core_ids_stage2_j784s4_evm=(12 13 14 15 18 19 20 21)
                core_ids_stage3_j784s4_evm=(0)
            elif [ $BOARD_NAME == "j742s2_evm" ]
            then
                # elf images list per stage
                elf_images_list_stage1_j742s2_evm=("bootapp_boot_test_j742s2_evm_mcu2_0TestApp_release.xer5f" \
                                                  "bootapp_boot_test_j742s2_evm_mcu2_1TestApp_release.xer5f" )
                elf_images_list_stage2_j742s2_evm=("bootapp_boot_test_j742s2_evm_mcu3_0TestApp_release.xer5f" \
                                                "bootapp_boot_test_j742s2_evm_mcu3_1TestApp_release.xer5f" \
                                                "bootapp_boot_test_j742s2_evm_mcu4_0TestApp_release.xer5f" \
                                                "bootapp_boot_test_j742s2_evm_mcu4_1TestApp_release.xer5f" \
                                                "bootapp_boot_test_j742s2_evm_c7x_1TestApp_release.xe71" \
                                                "bootapp_boot_test_j742s2_evm_c7x_2TestApp_release.xe71" \
                                                "bootapp_boot_test_j742s2_evm_c7x_3TestApp_release.xe71")
                elf_images_list_stage3_j742s2_evm=("bootapp_boot_test_j742s2_evm_mpu1_0TestApp_release.xa72fg")

                # RPRC files list per stage
                rprc_file_list_stage1_j742s2_evm=("bootapp_boot_test_j742s2_evm_mcu2_0TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j742s2_evm_mcu2_1TestApp_release.xer5f.rprc")
                rprc_file_list_stage2_j742s2_evm=("bootapp_boot_test_j742s2_evm_mcu3_0TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j742s2_evm_mcu3_1TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j742s2_evm_mcu4_0TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j742s2_evm_mcu4_1TestApp_release.xer5f.rprc" \
                                                "bootapp_boot_test_j742s2_evm_c7x_1TestApp_release.xe71.rprc" \
                                                "bootapp_boot_test_j742s2_evm_c7x_2TestApp_release.xe71.rprc" \
                                                "bootapp_boot_test_j742s2_evm_c7x_3TestApp_release.xe71.rprc")
                rprc_file_list_stage3_j742s2_evm=("bootapp_boot_test_j742s2_evm_mpu1_0TestApp_release.xa72fg.rprc")

                core_ids_stage1_j742s2_evm=(10 11)
                core_ids_stage2_j742s2_evm=(12 13 14 15 18 19 20)
                core_ids_stage3_j742s2_evm=(0)
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

            if [ ${BOARD_NAME} != "j7200_evm" ]
            then
                # Generate output arguments for Stage 3 appimage
                output_args3=()
                core_ids_list=core_ids_stage3_${BOARD_NAME}[@]
                core_ids_list=(${!core_ids_list})
                rprc_file_list=rprc_file_list_stage3_${BOARD_NAME}[@]
                rprc_file_list=(${!rprc_file_list})
                arrayLength=${#core_ids_list[@]}
                echo "Stage 3 length is $arrayLength"
                for (( i = 0; i < $arrayLength; i++ )); do
                    output_args3+="${core_ids_list[i]} ${rprc_file_list[i]} "
                done
            fi
            
            rm -rf ../multicore_images/${BOARD_NAME}/*
            mkdir -p ../multicore_images/${BOARD_NAME}
            MULTICOREAPP_BIN_PATH=${PDK_INSTALL_PATH}/packages/ti/boot/sbl/example/boot_app/multicore_images/${BOARD_NAME}
            pushd ${PWD}
            cd ${MULTICOREAPP_BIN_PATH}

            # Generate stage1 rprc files
            elf_images_list=elf_images_list_stage1_${BOARD_NAME}[@]
            elf_images_list=(${!elf_images_list})
            for i in ${elf_images_list[@]}; do
                # Use mono execute windows application in linux machine
                mono $SBL_OUT2RPRC_GEN_TOOL_PATH/out2rprc.exe ${PDK_INSTALL_PATH}/packages/ti/boot/sbl/example/k3MulticoreApp/binary/${BOARD_NAME%_*}/$i "$i.rprc"
            done

            # Generate stage2 rprc files
            elf_images_list=elf_images_list_stage2_${BOARD_NAME}[@]
            elf_images_list=(${!elf_images_list})
            for i in ${elf_images_list[@]}; do
                # Use mono execute windows application in linux machine
                mono $SBL_OUT2RPRC_GEN_TOOL_PATH/out2rprc.exe ${PDK_INSTALL_PATH}/packages/ti/boot/sbl/example/k3MulticoreApp/binary/${BOARD_NAME%_*}/$i "$i.rprc"
            done

            # Generate stage3 rprc files
            if [ $BOARD_NAME != "j7200_evm" ]
            then
                elf_images_list=elf_images_list_stage3_${BOARD_NAME}[@]
                elf_images_list=(${!elf_images_list})
                for i in ${elf_images_list[@]}; do
                    # Use mono execute windows application in linux machine
                    mono $SBL_OUT2RPRC_GEN_TOOL_PATH/out2rprc.exe ${PDK_INSTALL_PATH}/packages/ti/boot/sbl/example/k3MulticoreApp/binary/${BOARD_NAME%_*}/$i "$i.rprc"
                done
            fi

            get_appimage_names "${BOARD_NAME}"
            # Generate and sign lateapp1
            $MULTICORE_APPIMAGE_GEN_TOOL_PATH/MulticoreImageGen LE $devId $appImageName1 $output_args1
            ${PDK_INSTALL_PATH}/packages/ti/build/makerules/x509CertificateGen.sh -b ${PDK_INSTALL_PATH}/packages/ti/boot/sbl/example/boot_app/multicore_images/${BOARD_NAME}/$appImageName1 -o $MULTICOREAPP_BIN_PATH/$appImageName1.signed -c R5 -l 0x41C00100 -k ${PDK_INSTALL_PATH}/packages/ti/build/makerules/k3_dev_mpk.pem
            ${PDK_INSTALL_PATH}/packages/ti/build/makerules/x509CertificateGen.sh -b ${PDK_INSTALL_PATH}/packages/ti/boot/sbl/example/boot_app/multicore_images/${BOARD_NAME}/$appImageName1 -o $MULTICOREAPP_BIN_PATH/$appImageName1.hs_fs -c R5 -l 0x41C00100 -k ${PDK_INSTALL_PATH}/packages/ti/build/makerules/rom_degenerateKey.pem
            # Generate and sign lateapp2
            $MULTICORE_APPIMAGE_GEN_TOOL_PATH/MulticoreImageGen LE $devId $appImageName2 $output_args2
            ${PDK_INSTALL_PATH}/packages/ti/build/makerules/x509CertificateGen.sh -b ${PDK_INSTALL_PATH}/packages/ti/boot/sbl/example/boot_app/multicore_images/${BOARD_NAME}/$appImageName2 -o $MULTICOREAPP_BIN_PATH/$appImageName2.signed -c R5 -l 0x41C00100 -k ${PDK_INSTALL_PATH}/packages/ti/build/makerules/k3_dev_mpk.pem
            ${PDK_INSTALL_PATH}/packages/ti/build/makerules/x509CertificateGen.sh -b ${PDK_INSTALL_PATH}/packages/ti/boot/sbl/example/boot_app/multicore_images/${BOARD_NAME}/$appImageName2 -o $MULTICOREAPP_BIN_PATH/$appImageName2.hs_fs -c R5 -l 0x41C00100 -k ${PDK_INSTALL_PATH}/packages/ti/build/makerules/rom_degenerateKey.pem
            # Generate and sign lateapp3
            if [ $BOARD_NAME != "j7200_evm" ]
            then
                $MULTICORE_APPIMAGE_GEN_TOOL_PATH/MulticoreImageGen LE $devId $appImageName3 $output_args3
                ${PDK_INSTALL_PATH}/packages/ti/build/makerules/x509CertificateGen.sh -b ${PDK_INSTALL_PATH}/packages/ti/boot/sbl/example/boot_app/multicore_images/${BOARD_NAME}/$appImageName3 -o $MULTICOREAPP_BIN_PATH/$appImageName3.signed -c R5 -l 0x41C00100 -k ${PDK_INSTALL_PATH}/packages/ti/build/makerules/k3_dev_mpk.pem
                ${PDK_INSTALL_PATH}/packages/ti/build/makerules/x509CertificateGen.sh -b ${PDK_INSTALL_PATH}/packages/ti/boot/sbl/example/boot_app/multicore_images/${BOARD_NAME}/$appImageName3 -o $MULTICOREAPP_BIN_PATH/$appImageName3.hs_fs -c R5 -l 0x41C00100 -k ${PDK_INSTALL_PATH}/packages/ti/build/makerules/rom_degenerateKey.pem
            fi
            popd
        done

    fi
}

set_board_list
build_bootapp_boot_test
generate_lateapps

