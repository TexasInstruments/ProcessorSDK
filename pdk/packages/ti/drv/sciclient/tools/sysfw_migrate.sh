#!/bin/bash
#
# Copyright (c) 2018-2022, Texas Instruments Incorporated
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# *  Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# *  Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# *  Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Usage : sysfw_migrate.sh <release tag> [OPTIONS]

################################################################################
export RM=rm
export MV=mv
export MKDIR=mkdir
export MAKE=gcc
export ECHO=echo
export CHMOD=chmod
export COPY=cp
export CAT=cat

################################################################################
# Parse CLI arguments
SOC_LIST="j721e j7200 j721s2 j784s4 j742s2"
version=""
for i in "$@"; do
case $i in
    -sr|--skip-reset) # Skips the PDK reset and rebase step
        SKIP_RESET=YES
        shift
        ;;
    -sb|--skip-build) # Skips the sciclient binaries build step
        SKIP_BUILD=YES
        shift
        ;;
    -sc|--skip-commit) # Skips the PDK commit step
        SKIP_COMMIT=YES
        shift
        ;;
    --soc=*) #List of SOC's
        if [ "${i#*=}" != "" ]; then
            SOC_LIST="${i#*=}"
        fi
        shift
        ;;
    -h|--help)
        $ECHO "Usage : sysfw_migrate.sh <release tag> [OPTIONS]"
        $ECHO
        $ECHO "OPTIONS:-"
        $ECHO " -sr or --skip-reset           : Skips the PDK reset and rebase step"
        $ECHO " -sb or --skip-build           : Skips the sciclient binaries build step"
        $ECHO " -sg or --skip-gen             : Skips the firmwareHeaderGen.sh step"
        $ECHO " -sc or --skip-commit          : Skips the PDK commit step"
        $ECHO " --soc=\"\<soc_list\>\" : List of SOCs. Default will be all supported SOCs"
        $ECHO "     Supported SOCs:-"
        $ECHO "     - j721e"
        $ECHO "     - j7200"
        $ECHO "     - j721s2"
        $ECHO "     - j784s4"
        $ECHO "    For example, --soc=\"j721e\" or  --soc=\"j721e j7200\""
        exit 0
        ;;
    v*.*.*)
        $ECHO "$i"
        version="$i"
        shift
        ;;
    -*) # Invalid flag
        $ECHO "!!!WARNING!!! - IGNORING INVALID FLAG: $1"
        shift
        ;;
esac
done

################################################################################
# Specify paths relative to script
export SCRIPT_DIR=$(cd "$( dirname "${BASH_SOURCE[0]}")" && pwd )
export SCI_CLIENT_DIR=$(cd "$SCRIPT_DIR/.." && pwd )
export ROOTDIR=$(cd "$SCI_CLIENT_DIR/../../.." && pwd )
export PDK_DIR=$(cd "$ROOTDIR/.." && pwd )

$ECHO " Starting DM Migration for $SOC_LIST "

################################################################################
# Rebase to PDK master

if [ "$SKIP_RESET" != "YES" ]; then
    $ECHO "Reset PDK branch and rebase onto master"
    git reset --hard HEAD
    git fetch origin; git rebase origin/master

    cd $PDK_DIR/docs/
    $ECHO "Reset PDK_DOCS branch and rebase onto master"
    git reset --hard HEAD
    git fetch origin; git rebase origin/master
fi

################################################################################
# Build sciclient_ccs_init for use with launch.js
if [ "$SKIP_BUILD" != "YES" ]; then
    cd $ROOTDIR/ti/build

    for SOC in $SOC_LIST
    do
        make -j -s allclean
        if [ "$SOC" != "j742s2" ]; then
            make -j -s sciclient_boardcfg BOARD="$SOC"_evm
            make -j -s sciclient_boardcfg BOARD="$SOC"_evm BUILD_HS=yes
        fi
        make -j -s sciclient_ccs_init_clean BOARD="$SOC"_evm
        make -j -s sciclient_ccs_init BOARD="$SOC"_evm
        make -j -s sciserver_testapp_freertos_clean BOARD="$SOC"_evm
        make -j -s sciserver_testapp_freertos BOARD="$SOC"_evm
        $COPY $ROOTDIR/ti/binary/sciclient_ccs_init/bin/"$SOC"/sciclient_ccs_init_mcu1_0_release.xer5f $SCI_CLIENT_DIR/tools/ccsLoadDmsc/"$SOC"/
        $COPY $ROOTDIR/ti/binary/sciserver_testapp_freertos/bin/"$SOC"/sciserver_testapp_freertos_mcu1_0_release.xer5f $SCI_CLIENT_DIR/tools/ccsLoadDmsc/"$SOC"/
        $COPY $ROOTDIR/ti/binary/sciserver_testapp_freertos/bin/"$SOC"/sciserver_testapp_freertos_mcu1_0_release.rprc $SCI_CLIENT_DIR/tools/ccsLoadDmsc/"$SOC"/

        make -j -s sciserver_testapp_safertos_clean BOARD="$SOC"_evm
        make -j -s sciserver_testapp_safertos BOARD="$SOC"_evm

        if [ "$SOC" = "j7200" ] || [ "$SOC" = "j721s2" ] || [ "$SOC" = "j784s4" ]; then
            make -j -s sciclient_boardcfg_combined BOARD="$SOC"_evm
        fi
    done

    cd -
fi

################################################################################
# Commit changes to PDK
if [ "$SKIP_COMMIT" != "YES" ]; then
    $ECHO "Commit changes to PDK"
    cd $SCRIPT_DIR

    for SOC in $SOC_LIST
    do
        case $SOC in
            "j721e")
                git add $SCI_CLIENT_DIR/soc/V1
                git add $SCI_CLIENT_DIR/tools/ccsLoadDmsc/j721e
                shift
                ;;
            "j7200")
                git add $SCI_CLIENT_DIR/soc/V2
                git add $SCI_CLIENT_DIR/tools/ccsLoadDmsc/j7200
                shift
                ;;
            "j721s2")
                git add $SCI_CLIENT_DIR/soc/V4
                git add $SCI_CLIENT_DIR/tools/ccsLoadDmsc/j721s2
                shift
                ;;
            "j784s4")
                git add $SCI_CLIENT_DIR/soc/V6
                git add $SCI_CLIENT_DIR/tools/ccsLoadDmsc/j784s4
                shift
                ;;
            "j742s2")
                git add $SCI_CLIENT_DIR/soc/V6
                git add $SCI_CLIENT_DIR/tools/ccsLoadDmsc/j742s2
                shift
                ;;
        esac
    done

    git add $SCI_CLIENT_DIR/src/version/*

    git commit -s -m "Migrating to DM version $version "
fi


################################################################################

$ECHO "Done."
