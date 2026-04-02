#!/bin/bash
#
# Copyright (c) 2025, Texas Instruments Incorporated
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
# Usage : ./sbl_tifs_sign.sh --version=11.01.00 [OPTIONS]

################################################################################
export ECHO=echo

################################################################################
# Parse CLI arguments
SOC_LIST="j721e j7200 j721s2 j784s4 j742s2"
version=""
for i in "$@"; do
case $i in
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
    --version=*) #List of SOC's
        version="${1#*=}"
        shift
        ;;
    -h|--help)
        $ECHO "Usage : sbl_tifs_sign.sh --version=<release tag> [OPTIONS]"
        $ECHO
        $ECHO "OPTIONS:-"
        $ECHO " -sc or --skip-commit          : Skips the PDK commit step"
        $ECHO " --soc=\"\<soc_list\>\" : List of SOCs. Default will be all supported SOCs"
        $ECHO "     Supported SOCs:-"
        $ECHO "     - j721e"
        $ECHO "     - j7200"
        $ECHO "     - j721s2"
        $ECHO "     - j784s4"
        $ECHO "     - j742s2"
        $ECHO "    For example, --soc=\"j721e\" or  --soc=\"j721e j7200\""
        exit 0
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

$ECHO "Starting Migration for $SOC_LIST"


################################################################################

if [ "$SKIP_GEN_BIN" != "YES" ];  then

    cd $ROOTDIR/ti/drv/sciclient/tools/

    for SOC in $SOC_LIST
    do
        case $SOC in
            "j721e")
                ./firmwareHeaderGen.sh j721e
                ./firmwareHeaderGen.sh j721e-hs
                ./firmwareHeaderGen.sh j721e_sr1_1-hs
                ./firmwareHeaderGen.sh j721e_sr2-hs
                ./firmwareHeaderGen.sh j721e_sr2-hs-fs
                shift
                ;;
            "j7200")
                ./firmwareHeaderGen.sh j7200
                ./firmwareHeaderGen.sh j7200-hs
                ./firmwareHeaderGen.sh j7200_sr2-hs
                ./firmwareHeaderGen.sh j7200_sr2-hs-fs
                shift
                ;;
            "j721s2")
                ./firmwareHeaderGen.sh j721s2
                ./firmwareHeaderGen.sh j721s2-hs
                ./firmwareHeaderGen.sh j721s2-hs-fs
                shift
                ;;
            "j784s4")
                ./firmwareHeaderGen.sh j784s4
                ./firmwareHeaderGen.sh j784s4-hs
                ./firmwareHeaderGen.sh j784s4-hs-fs
                shift
                ;;
        esac
    done

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
                shift
                ;;
            "j7200")
                git add $SCI_CLIENT_DIR/soc/V2
                shift
                ;;
            "j721s2")
                git add $SCI_CLIENT_DIR/soc/V4
                shift
                ;;
            "j784s4")
                git add $SCI_CLIENT_DIR/soc/V6
                shift
                ;;
            "j742s2")
                git add $SCI_CLIENT_DIR/soc/V6
                shift
                ;;
        esac
    done

    git commit -s -m "Added signed binaries for TIFS version $version"
fi


################################################################################

$ECHO "Done."
