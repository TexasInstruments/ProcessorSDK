#!/bin/bash
#
# Copyright (c) 2018-2020, Texas Instruments Incorporated
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
# Usage : For AM65XX            : ./firmwareHeaderGen.sh am65x
#         For AM65XX            : ./firmwareHeaderGen.sh am65x_sr2
#         For AM65XX-HS         : ./firmwareHeaderGen.sh am65x-hs
#         For AM65XX-HS         : ./firmwareHeaderGen.sh am65x_sr2-hs
#         For J721E             : ./firmwareHeaderGen.sh j721e
#         For J721E-HS          : ./firmwareHeaderGen.sh j721e-hs
#         For J721E-HS (ES1.1)  : ./firmwareHeaderGen.sh j721e_sr1_1-hs
#         For J721E-HS (ES2.0)  : ./firmwareHeaderGen.sh j721E_sr2-hs
#         For J721E-HS Prime    : ./firmwareHeaderGen.sh j721e-hsp
#         For AM64x             : ./firmwareHeaderGen.sh am64x
#         For J7200             : ./firmwareHeaderGen.sh j7200
#         For J7200-HS          : ./firmwareHeaderGen.sh j7200-hs
#         For J7200-HS (ES2.0)  : ./firmwareHeaderGen.sh j7200_sr2-hs
#         For J7200-HS Prime    : ./firmwareHeaderGen.sh j7200-hsp
#         For J721S2            : ./firmwareHeaderGen.sh j721s2
#         For J721S2-HS         : ./firmwareHeaderGen.sh j721s2-hs
#         For J784S4            : ./firmwareHeaderGen.sh j784s4-zebu-combined
export RM=rm
export MV=mv
export MAKE=gcc
export ECHO=echo
export CHMOD=chmod
export COPY=cp
export CAT=cat
#Default SOC is am65x .This can be changed by using first parameter
# as ,for example, "j721e". Assumes device type is GP by default.
export FW_SOC=am65x
export FW_SOC_TYPE=-gp
export BIN_EXT=

if [[ $OS == 'Windows_NT' ]]; then
export BIN2C_EXE=bin2c.exe
else
export BIN2C_EXE=bin2c.out
fi

# Specify paths relative to script
export SCRIPT_DIR=$(cd "$( dirname "${BASH_SOURCE[0]}")" && pwd )
echo $SCRIPT_DIR
export SCI_CLIENT_DIR=$(cd "$SCRIPT_DIR/.." && pwd )

# Keeping everything within SCI client folder
export BIN2C_GEN=$SCI_CLIENT_DIR/tools/bin2c/$BIN2C_EXE

# ROOTDIR is used by x509CertificateGen.sh
# If not defined, try to get it relative to
# script location. This flexibility is useful
# when running this script from a git repo, outside
# PRSDK.
if [ "$#" -gt 1 ]; then
export ROOTDIR=$2
export FW_SOC=$1
elif [ "$#" -gt 0 ]; then
export ROOTDIR=$(cd "$SCI_CLIENT_DIR/../../.." && pwd )
export FW_SOC=$1
else
export ROOTDIR=$(cd "$SCI_CLIENT_DIR/../../.." && pwd )
fi

# Pickup correct sysfw binary
if [[ $FW_SOC == *"hs"* ]]; then
  FW_SOC_TYPE=-hs-enc
  FW_SOC=${FW_SOC%-hs}
  BIN_EXT=-hs-enc
fi

if [[ $FW_SOC == *"hsp"* ]]; then
  FW_SOC_TYPE=-hs
  FW_SOC=${FW_SOC%-hsp}
  BIN_EXT=-hsp
  IS_PRIME=y
fi

if [[ $FW_SOC == *"vlab"* ]]; then
  FW_SOC=${FW_SOC%-vlab}
  FW_SOC_TYPE=-gp-vlab
  BIN_EXT=-vlab
fi
if [[ $FW_SOC == *"zebu"* ]]; then
  FW_SOC=${FW_SOC%-zebu-combined}
  FW_SOC_TYPE=-gp-zebu-combined
  BIN_EXT=-zebu-combined
fi

if [[ $FW_SOC == *"hs-fs"* ]]; then
  FW_SOC_TYPE=-hs-fs-enc
  FW_SOC=${FW_SOC%-hs-fs}
  BIN_EXT=-hs-fs-enc
  IS_HS_FS=y
fi

export SCI_CLIENT_IN_SOC_DIR=$SCI_CLIENT_DIR/soc/sysfw/binaries

if [ "$FW_SOC" = "am65x" ]; then
export SCI_CLIENT_OUT_SOC_DIR=$SCI_CLIENT_DIR/soc/V0
export SCICLIENT_FIRMWARE_HEADER=sciclient_firmware_V0$BIN_EXT.h
export SYSFW_SE_SIGNED=$SCI_CLIENT_OUT_SOC_DIR/sysfw$BIN_EXT.bin
export FIRMWARE_SILICON=$SCI_CLIENT_IN_SOC_DIR/ti-sci-firmware-$FW_SOC$FW_SOC_TYPE.bin
export SYSFW_SE_INNER_CERT=$SCI_CLIENT_IN_SOC_DIR/ti-sci-firmware-$FW_SOC${FW_SOC_TYPE%-enc}-cert.bin
export SYSFW_SE_CUST_CERT=$SCI_CLIENT_OUT_SOC_DIR/sysfw_cert.bin
export SYSFW_LOAD_ADDR=0x40000
fi

if [ "$FW_SOC" = "am65x_sr2" ]; then
export SCI_CLIENT_OUT_SOC_DIR=$SCI_CLIENT_DIR/soc/V0
export SCICLIENT_FIRMWARE_HEADER=sciclient_firmware_V0_sr2$BIN_EXT.h
export SYSFW_SE_SIGNED=$SCI_CLIENT_OUT_SOC_DIR/sysfw_sr2$BIN_EXT.bin
export FIRMWARE_SILICON=$SCI_CLIENT_IN_SOC_DIR/ti-sci-firmware-$FW_SOC$FW_SOC_TYPE.bin
export SYSFW_SE_INNER_CERT=$SCI_CLIENT_IN_SOC_DIR/ti-sci-firmware-$FW_SOC${FW_SOC_TYPE%-enc}-cert.bin
export SYSFW_SE_CUST_CERT=$SCI_CLIENT_OUT_SOC_DIR/sysfw_cert.bin
export SYSFW_LOAD_ADDR=0x40000
fi

if [ "$FW_SOC" = "j721e" ]; then
export SCI_CLIENT_OUT_SOC_DIR=$SCI_CLIENT_DIR/soc/V1
export SCICLIENT_FIRMWARE_HEADER=sciclient_firmware_V1$BIN_EXT.h
export SYSFW_SE_SIGNED=$SCI_CLIENT_OUT_SOC_DIR/tifs$BIN_EXT.bin
export FIRMWARE_SILICON=$SCI_CLIENT_IN_SOC_DIR/ti-fs-firmware-$FW_SOC$FW_SOC_TYPE.bin
export SYSFW_SE_INNER_CERT=$SCI_CLIENT_IN_SOC_DIR/ti-fs-firmware-$FW_SOC${FW_SOC_TYPE%-enc}-cert.bin
export SYSFW_SE_CUST_CERT=$SCI_CLIENT_OUT_SOC_DIR/tifs_cert.bin
export SYSFW_LOAD_ADDR=0x40000
fi

if [ "$FW_SOC" = "j721e_sr1_1" ]; then
export SCI_CLIENT_OUT_SOC_DIR=$SCI_CLIENT_DIR/soc/V1
export SCICLIENT_FIRMWARE_HEADER=sciclient_firmware_V1_sr1.1$BIN_EXT.h
export SYSFW_SE_SIGNED=$SCI_CLIENT_OUT_SOC_DIR/tifs_sr1.1$BIN_EXT.bin
export FIRMWARE_SILICON=$SCI_CLIENT_IN_SOC_DIR/ti-fs-firmware-$FW_SOC$FW_SOC_TYPE.bin
export SYSFW_SE_INNER_CERT=$SCI_CLIENT_IN_SOC_DIR/ti-fs-firmware-$FW_SOC${FW_SOC_TYPE%-enc}-cert.bin
export SYSFW_SE_CUST_CERT=$SCI_CLIENT_OUT_SOC_DIR/tifs_cert_sr1.1.bin
export SYSFW_LOAD_ADDR=0x40000
fi

if [ "$FW_SOC" = "j721e_sr2" ]; then
export SCI_CLIENT_OUT_SOC_DIR=$SCI_CLIENT_DIR/soc/V1
export SCICLIENT_FIRMWARE_HEADER=sciclient_firmware_V1_sr2$BIN_EXT.h
export SYSFW_SE_SIGNED=$SCI_CLIENT_OUT_SOC_DIR/tifs_sr2$BIN_EXT.bin
export FIRMWARE_SILICON=$SCI_CLIENT_IN_SOC_DIR/ti-fs-firmware-$FW_SOC$FW_SOC_TYPE.bin
export SYSFW_SE_INNER_CERT=$SCI_CLIENT_IN_SOC_DIR/ti-fs-firmware-$FW_SOC${FW_SOC_TYPE%-enc}-cert.bin
export SYSFW_SE_CUST_CERT=$SCI_CLIENT_OUT_SOC_DIR/tifs_cert_sr2.bin
export SYSFW_LOAD_ADDR=0x40000
fi

if [ "$FW_SOC" = "j7200" ]; then
export SCI_CLIENT_OUT_SOC_DIR=$SCI_CLIENT_DIR/soc/V2
export SCICLIENT_FIRMWARE_HEADER=sciclient_firmware_V2$BIN_EXT.h
export FIRMWARE_SILICON=$SCI_CLIENT_IN_SOC_DIR/ti-fs-firmware-$FW_SOC$FW_SOC_TYPE.bin
export SYSFW_SE_INNER_CERT=$SCI_CLIENT_IN_SOC_DIR/ti-fs-firmware-$FW_SOC${FW_SOC_TYPE%-enc}-cert.bin
export SYSFW_SE_SIGNED=$SCI_CLIENT_OUT_SOC_DIR/tifs$BIN_EXT.bin
export SYSFW_SE_CUST_CERT=$SCI_CLIENT_OUT_SOC_DIR/tifs_cert.bin
export SYSFW_LOAD_ADDR=0x40000
fi

if [ "$FW_SOC" = "j7200_sr2" ]; then
export SCI_CLIENT_OUT_SOC_DIR=$SCI_CLIENT_DIR/soc/V2
export SCICLIENT_FIRMWARE_HEADER=sciclient_firmware_V2_sr2$BIN_EXT.h
export SYSFW_SE_SIGNED=$SCI_CLIENT_OUT_SOC_DIR/tifs_sr2$BIN_EXT.bin
export FIRMWARE_SILICON=$SCI_CLIENT_IN_SOC_DIR/ti-fs-firmware-$FW_SOC$FW_SOC_TYPE.bin
export SYSFW_SE_INNER_CERT=$SCI_CLIENT_IN_SOC_DIR/ti-fs-firmware-$FW_SOC${FW_SOC_TYPE%-enc}-cert.bin
export SYSFW_SE_CUST_CERT=$SCI_CLIENT_OUT_SOC_DIR/tifs_cert_sr2.bin
export SYSFW_LOAD_ADDR=0x40000
fi

if [ "$FW_SOC" = "am64x" ]; then
export SCI_CLIENT_OUT_SOC_DIR=$SCI_CLIENT_DIR/soc/V3
SYSFW_SE_SIGNED=$SCI_CLIENT_OUT_SOC_DIR/sysfw$BIN_EXT.bin
export SCICLIENT_FIRMWARE_HEADER=sciclient_firmware_V3$BIN_EXT.h
export FIRMWARE_SILICON=$SCI_CLIENT_IN_SOC_DIR/ti-sci-firmware-$FW_SOC$FW_SOC_TYPE.bin
export SYSFW_SE_INNER_CERT=$SCI_CLIENT_IN_SOC_DIR/ti-sci-firmware-$FW_SOC${FW_SOC_TYPE%-enc}-cert.bin
export SYSFW_SE_CUST_CERT=$SCI_CLIENT_OUT_SOC_DIR/sysfw_cert.bin
export SYSFW_LOAD_ADDR=0x44000
fi

if [ "$FW_SOC" = "j721s2" ]; then
export SCI_CLIENT_OUT_SOC_DIR=$SCI_CLIENT_DIR/soc/V4
export SCICLIENT_FIRMWARE_HEADER=sciclient_firmware_V4$BIN_EXT.h
export FIRMWARE_SILICON=$SCI_CLIENT_IN_SOC_DIR/ti-fs-firmware-$FW_SOC$FW_SOC_TYPE.bin
export SYSFW_SE_INNER_CERT=$SCI_CLIENT_IN_SOC_DIR/ti-fs-firmware-$FW_SOC${FW_SOC_TYPE%-enc}-cert.bin
export SYSFW_SE_SIGNED=$SCI_CLIENT_OUT_SOC_DIR/tifs$BIN_EXT.bin
export SYSFW_SE_CUST_CERT=$SCI_CLIENT_OUT_SOC_DIR/tifs_cert.bin
export SYSFW_LOAD_ADDR=0x40000
fi

if [ "$FW_SOC" = "j784s4" ]; then
export SCI_CLIENT_OUT_SOC_DIR=$SCI_CLIENT_DIR/soc/V6
export SCICLIENT_FIRMWARE_HEADER=sciclient_firmware_V6$BIN_EXT.h
export FIRMWARE_SILICON=$SCI_CLIENT_IN_SOC_DIR/ti-fs-firmware-$FW_SOC$FW_SOC_TYPE.bin
export SYSFW_SE_INNER_CERT=$SCI_CLIENT_IN_SOC_DIR/ti-fs-firmware-$FW_SOC${FW_SOC_TYPE%-enc}-cert.bin
export SYSFW_SE_SIGNED=$SCI_CLIENT_OUT_SOC_DIR/tifs$BIN_EXT.bin
export SYSFW_SE_CUST_CERT=$SCI_CLIENT_OUT_SOC_DIR/tifs_cert.bin
export SYSFW_LOAD_ADDR=0x40000
fi

if [ "$IS_PRIME" = "y" ]; then
    # HS Prime devices rely on customer-built/signed TIFS image
    if [ -z "$TIFS_DIR" ]; then
        echo "Error - did not set TIFS directory"
        exit 1
    fi
    if [ ! -d $TIFS_DIR ]; then
        echo "Error: specified TIFS directory does not exist"
        exit 1
    fi

    export FIRMWARE_SILICON=$TIFS_DIR/ti-fs-firmware-$FW_SOC$FW_SOC_TYPE.bin
    if [ ! -f $FIRMWARE_SILICON ]; then
        echo "Error: TIFS binary $FIRMWARE_SILICON does not exist"
        exit 1
    fi
fi

# SBL_CERT_GEN may already be depending on how this is called
export SBL_CERT_GEN="${SBL_CERT_GEN:-$ROOTDIR/ti/build/makerules/x509CertificateGen.sh}"

# Confirm ROOTDIR is correct, if not error out.
if [ ! -f $SBL_CERT_GEN ]; then
    echo "Error: $SBL_CERT_GEN not found!"
    echo "       Usage $0 <pdk-install-path>"
    exit 1
fi

$ECHO "Building the bin2c generation c tool"
cd $SCI_CLIENT_DIR/tools/bin2c/
$RM -f $BIN2C_EXE
$MAKE bin2c.c -o $BIN2C_EXE
cd -

$CHMOD a+x $SBL_CERT_GEN
$CHMOD a+x $BIN2C_GEN

if [ "$IS_PRIME" = "y" ]; then
    # Only single-signed firmware, no outer certificate for 
    $ECHO "Generating firmware certificate for " $FIRMWARE_SILICON
    export FIRMWARE_CERT_KEY=$ROOTDIR/ti/build/makerules/k3_dev_mpk.pem
    export FIRMWARE_ENC_KEY=$ROOTDIR/ti/build/makerules/k3_dev_mek.txt
    $SBL_CERT_GEN -b $FIRMWARE_SILICON -o $SYSFW_SE_SIGNED -c DMSC_I -l $SYSFW_LOAD_ADDR -k $FIRMWARE_CERT_KEY -y ENCRYPT -e $FIRMWARE_ENC_KEY
    #$SBL_CERT_GEN -b $FIRMWARE_SILICON -o $SYSFW_SE_SIGNED -c DMSC_I -l $SYSFW_LOAD_ADDR -k $FIRMWARE_CERT_KEY
else
    if [[ $FW_SOC == *"gp"* || $FW_SOC_TYPE == *"gp"* ]]; then
    $ECHO "Generating the Header file for " $FIRMWARE_SILICON
    export SBL_CERT_KEY=$ROOTDIR/ti/build/makerules/rom_degenerateKey.pem
    $SBL_CERT_GEN -b $FIRMWARE_SILICON -o $SYSFW_SE_SIGNED -c DMSC_I -l $SYSFW_LOAD_ADDR -k $SBL_CERT_KEY
    else
    if [ "$IS_HS_FS" = "y" ]; then
      $ECHO "Skipping Outer Certificate generation for HS-FS device"
    else
      $ECHO "Generating outer certificate for " $SYSFW_SE_INNER_CERT
      export SBL_CERT_KEY=$ROOTDIR/ti/build/makerules/k3_dev_mpk.pem
      $SBL_CERT_GEN -b $SYSFW_SE_INNER_CERT -o $SYSFW_SE_CUST_CERT -c DMSC_O -l $SYSFW_LOAD_ADDR -k $SBL_CERT_KEY
    fi

    $ECHO "Generating the Header file for " $FIRMWARE_SILICON

    if [ "$IS_HS_FS" = "y" ]; then
      $CAT $SYSFW_SE_INNER_CERT $FIRMWARE_SILICON > $SYSFW_SE_SIGNED
    else
      $CAT $SYSFW_SE_CUST_CERT $FIRMWARE_SILICON > $SYSFW_SE_SIGNED
      $RM -f $SYSFW_SE_CUST_CERT
    fi
    fi
fi
$ECHO "Generating the Header file for the soc in the folder"
#Replacing '-' with '_' in the header file names and macros defined within
SCICLIENT_FIRMWARE_HEADER_FILE="${SCICLIENT_FIRMWARE_HEADER//-/_}"
$BIN2C_GEN $SYSFW_SE_SIGNED $SCI_CLIENT_OUT_SOC_DIR/$SCICLIENT_FIRMWARE_HEADER_FILE SCICLIENT_FIRMWARE 

$ECHO "Done."

