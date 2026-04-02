#!/bin/bash -e
#
# Copyright (c) 2023, Texas Instruments Incorporated
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
# Usage : lpm_s2r_gen.sh
#
################################################################################
trap cleanup EXIT

cleanup(){
    retcode="${?}"
    [ "$retcode" = "0" ] || {
        echo "Error detected !!"
        exit 1
    }
}

################################################################################
export ECHO="echo"

# Specify paths relative to script
export SCRIPT_DIR=$(cd "$( dirname "${BASH_SOURCE[0]}")" && pwd )
export LPM_DIR=$(cd "$SCRIPT_DIR/.." && pwd )
export ROOTDIR=$(cd "$LPM_DIR/../../.." && pwd )
export PDK_DIR=$(cd "$ROOTDIR/.." && pwd )

: ${CROSS_COMPILE:="arm-linux-gnueabihf-"}
LPM_S2R_LD=${CROSS_COMPILE}ld
LPM_S2R_OBJCOPY=${CROSS_COMPILE}objcopy
BIN2C_GEN="$ROOTDIR/ti/drv/sciclient/tools/bin2c/bin2c.out"

################################################################################
# Parse CLI arguments
SOC_LIST="j7200 j784s4 j742s2"
for i in "$@"; do
case $i in
    -sr|--skip-reset) # Skips the PDK reset and rebase step
        SKIP_RESET=YES
        shift
        ;;
    -sb|--skip-build) # Skips the sciclient_ccs_init build step
        SKIP_BUILD=YES
        shift
        ;;
    -sc|--skip-commit) # Skips the PDK commit step
        SKIP_COMMIT=YES
        shift
        ;;
    -*) # Invalid flag
        $ECHO "!!!WARNING!!! - IGNORING INVALID FLAG: $1"
        shift
        ;;
esac
done

################################################################################
# Reset PDK

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
# Build lpm_s2r object file
if [ "$SKIP_BUILD" != "YES" ]; then
    cd "$ROOTDIR/ti/build"

    for SOC in $SOC_LIST
    do
        make -j -s allclean
        make -j -s lpm_s2r BOARD="$SOC"_evm COMPILE_MODE="arm"
        OBJDIR="$(find $ROOTDIR/ti/binary/ti/drv/lpm/obj/${SOC}_evm/*/release -maxdepth 0 -type d)"
        $LPM_S2R_LD -Bstatic -T "$SCRIPT_DIR/s2r.lds" "$(ls $OBJDIR/lpm_s2r.o*)" -o "$OBJDIR/lpm_s2r.elf"
        $LPM_S2R_OBJCOPY -S -O binary "$OBJDIR/lpm_s2r.elf" "$OBJDIR/lpm_s2r.bin"

        mkdir -p "$LPM_DIR/src/s2r/${SOC}_evm"
        $BIN2C_GEN "$OBJDIR/lpm_s2r.bin" "$LPM_DIR/src/s2r/${SOC}_evm/lpm_s2r.h" lpm_sram_s2r
    done

    cd -
fi

################################################################################
# Commit changes to PDK
if [ "$SKIP_COMMIT" != "YES" ]; then
    $ECHO "Commit changes to PDK"
    cd $LPM_DIR
    for SOC in $SOC_LIST
    do
        git add "src/s2r/${SOC}_evm/lpm_s2r.h"
    done
    git commit -s -m "Update lpm_s2r.h for $SOC_LIST"
fi

################################################################################
$ECHO "Done."
