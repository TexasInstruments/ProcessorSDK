#!/bin/bash
#
# Copyright (c) 2018, Texas Instruments Incorporated
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
# K3ROM signing script to generate x509 certificate to binary

#variables
TEMP_X509=./x509-temp.cfg
TSIGN_KEY=rsa4k.pem
OUTPUT=x509-firmware.bin
VALID_CERT="R5 DMSC_I DMSC_O"
VALID_ENC="ENCRYPT DKEY_ENCRYPT"
VALID_DBG_TYPES="DBG_PERM_DISABLE DBG_SOC_DEFAULT DBG_PUBLIC_ENABLE DBG_FULL_ENABLE"
VALID_R5_BOOTCORE_OPTS="EFUSE_DEFAULT SPLIT_MODE"
#Defaults
SHA=sha512
CERT_SIGN=R5
X509_DEFAULT=x509-base.cfg

LOADADDR=0
SWRV=1
BOARD_CONFIG=0
ENC_IV_VAL=0000
ENC_RS_VAL=0000
ENC_KD_INDEX_VAL=0
ENC_KD_SALT_VAL=0000
R5_BOOTCORE_OPTS=0

DEVICE_UID=0000000000000000000000000000000000000000000000000000000000000000
DEVICE_DEBUG_TYPE=0000

declare -A sha_oids
sha_oids["sha256"]=2.16.840.1.101.3.4.2.1
sha_oids["sha384"]=2.16.840.1.101.3.4.2.2
sha_oids["sha512"]=2.16.840.1.101.3.4.2.3
sha_oids["sha224"]=2.16.840.1.101.3.4.2.4

gen_ecdsakey() {
	openssl ecparam -out $TSIGN_KEY -name prime256v1 -genkey
	SIGN_KEY=$TSIGN_KEY
}

gen_rsakey() {
	openssl genrsa -out $TSIGN_KEY 4096
	SIGN_KEY=$TSIGN_KEY
}

gen_enckey() {
	echo "Generating Random Encryption Key of 32 bytes:"
	openssl rand -hex 32 > enckey.txt
	ENC_KEY=enckey.txt
}

gen_enciv() {
	echo "Generating Random Encryption IV of 16 bytes:"
	openssl rand -hex 16 > enciv.txt
	ENC_IV=enciv.txt
}

gen_encrs() {
	echo "Generating Random Tail String of 32 bytes:"
	openssl rand -hex 32 > encrs.txt
	ENC_RS=encrs.txt
}

gen_encsalt() {
	echo "Generating SALT for HKDF2 Key generation of 32 bytes:"
	openssl rand -hex 32 > encsalt.txt
	ENC_SALT=encsalt.txt
}

image_encrypt() {
	echo
	echo "Image Encryption is enabled :"
	if [ -z "$ENC_KEY" ]; then
		gen_enckey
		echo "Note: Encryption key is available in enckey.txt:"
	fi

	if [ -z "$ENC_IV" ]; then
		gen_enciv
	fi

	if [ -z "$ENC_RS" ]; then
		gen_encrs
	fi

	ENC_IV_VAL=`cat $ENC_IV`
	ENC_RS_VAL=`cat $ENC_RS`
	echo "ENC_KEY = $ENC_KEY"
	echo "ENC_IV  = $ENC_IV_VAL"
	echo "ENC_RS  = $ENC_RS_VAL"
	cp $BIN  enc_tmp.bin
	truncate -s %16 enc_tmp.bin
	xxd -r -p $ENC_RS enc_rs.bin
	cat enc_tmp.bin  enc_rs.bin > enc_bin_rs.bin
	ENC_BIN=$CERT_SIGN"-ENC-"$(basename $BIN)
	echo "$ENC_BIN"
	if [ "$IMG_ENC" == "ENCRYPT" ];then
		openssl aes-256-cbc -e -K `cat $ENC_KEY` -iv $ENC_IV_VAL -in enc_bin_rs.bin -out $ENC_BIN -nopad
	else
		echo
		echo "Deriving Encryption Key using HKDF2 scheme :"
		if [ -z "$ENC_SALT" ]; then
			gen_encsalt
		fi
		ENC_KD_INDEX_VAL=1
		ENC_KD_SALT_VAL=`cat $ENC_SALT`
		echo "	ENC_KEY_INDEX  = $ENC_KD_INDEX_VAL"
		echo "	ENC_KEY_SALT  = $ENC_KD_SALT_VAL"
		xxd -r -p $ENC_KEY enc_key.bin
		echo -n '01'| xxd -r -p > 1.bin
		openssl dgst -sha512 -mac hmac -macopt hexkey:`cat $ENC_SALT` enc_key.bin | sed -e 's/.* //g' > encprk.txt
		openssl dgst -sha512 -mac hmac -macopt hexkey:`cat encprk.txt` 1.bin | sed -e 's/.* //g' > encgenkey.txt
		openssl aes-256-cbc -e -K `head -c 64 encgenkey.txt` -iv $ENC_IV_VAL -in enc_bin_rs.bin -out $ENC_BIN -nopad
	fi
	echo "BINARY ENCRYPTION SUCCESS: Generated $ENC_BIN"
	echo
	echo
}

declare -A options_help
usage() {
	if [ -n "$*" ]; then
		echo "ERROR: $*"
	fi
	echo -n "Usage: $0 "
	for option in "${!options_help[@]}"
	do
		arg=`echo ${options_help[$option]}|cut -d ':' -f1`
		if [ -n "$arg" ]; then
			arg=" $arg"
		fi
		echo -n "[-$option$arg] "
	done
	echo
	echo -e "\nWhere:"
	for option in "${!options_help[@]}"
	do
		arg=`echo ${options_help[$option]}|cut -d ':' -f1`
		txt=`echo ${options_help[$option]}|cut -d ':' -f2`
		tb="\t\t\t"
		if [ -n "$arg" ]; then
			arg=" $arg"
			tb="\t"
		fi
		echo -e "   -$option$arg:$tb$txt"
	done
	echo
	echo "Examples of usage:-"
	echo "# Generate x509 certificate for R5 SBL binaries (R5-cert.bin will be created)"
	echo "    $0 -b r5-sbl.bin -c R5 -k rsakey.pem "
	echo "# Encrypt and Generate x509 certificate for R5 SBL binaries (R5-cert.bin R5-ENC-*.bin will be created)"
	echo "    $0 -b r5-sbl.bin -c R5 -k rsakey.pem -y ENCRYPT -e enckey.txt -i enciv.txt -t encrs.txt"

	echo "# Generate x509 certificate for DMSC FW binaries (DMSC_I-cert.bin will be created)"
	echo "    $0 -b sys-fw.bin -c DMSC_I -k rsakey.pem "
	echo "# Encrypt and Generate x509 certificate for SYS FW binaries (DMSC_I-cert.bin DMSC_I-ENC-*.bin will be created)"
	echo "    $0 -b sys-fw.bin -c DMSC_I -k rsakey.pem -y ENCRYPT -e enckey.txt -i enciv.txt -t encrs.txt"
	echo "# Generate x509 certificate for DMSC Inner certficiate (DMSC_O-cert.bin will be created)"
	echo "    $0 -b DMSC_I-cert.bin -c DMSC_O -k rsakey.pem "

}

options_help[b]="bin_file:Bin file that needs to be signed"
options_help[k]="sign_key_file:Siging Key file with key value(PEM). If not provided script generates a random RSA 4K key."
options_help[o]="output_file:Name of the final output file. default x509-firmware.bin"
options_help[c]="cert_type:Target core on which the image would be running. Default is R5. Valid option are $VALID_CERT"
options_help[l]="loadaddr:Target load address of the binary in hex. Default R5:0x41C00000 DMSC:0x00040000"
options_help[s]="SWRV:SWRV value to be included in Certificate supported values (0-47). Default value $SWRV"
options_help[f]="BOARD_CONFIG:Option to indicate this is a board configuration value. Supported values (0-1). Default value $BOARD_CONFIG"
options_help[y]="image_encryption:Default is disabled. Valid options are $VALID_ENC"
options_help[e]="enc_key_file:txt file with key inside it. If not provided script generates a random key."
options_help[i]="enc_iv_file:txt file with Encryption Initial Vector(16 bytes) inside it. If not provided script generates a random IV."
options_help[r]="enc_rs_file:txt file with Encryption Tail String(32 bytes) inside it. If not provided script generates a random string."
options_help[d]="debug_extn:Default is disabled. Valid option DEBUG"
options_help[u]="UID_file:txt file with Device Specific Unique ID(32 bytes) included in Debug Extn. Default will be 0"
options_help[j]="debug_type:Debug type to be included in Debug Extn. Valid options are $VALID_DBG_TYPES"
options_help[m]="mode:Start R5 in lockstep or split mode. Valid options are $VALID_R5_BOOTCORE_OPTS"
options_help[a]="SBL_IMAGE_TYPE: Option to indicate that a combined boot image will be created"
options_help[w]="SYSFW: Bin file corresponding to sysfw image"
options_help[v]="SYSFW loadaddress: SYSFW image load address"
options_help[g]="SYSFW_DATA: Bin file corresponding to combined board configurations"
options_help[n]="SYSFW_DATA loadaddr: Combine board configuration load address"
options_help[t]="DM_DATA: Bin file corresponding to combined board configurations for RM and PM. If this is used, RM and PM do not need to be provided as part of SYSFW_DATA. (OPTIONAL)"
options_help[p]="DM_DATA loadaddr: Combine RM and PM blob board configuration load address (OPTIONAL)"
options_help[q]="SYSFW CERT: SYSFW Inner Certificate"

while getopts "b:k:o:c:l:s:e:i:r:y:d:u:j:m:a:w:v:g:n:t:p:q:h:f" opt
do
	case $opt in
	b)
		BIN=$OPTARG
	;;
	k)
		SIGN_KEY=$OPTARG
    ;;
    o)
        OUTPUT=$OPTARG
	;;
	e)
		ENC_KEY=$OPTARG
	;;
	i)
		ENC_IV=$OPTARG
	;;
	r)
		ENC_RS=$OPTARG
	;;
	l)
		LOADADDR=$OPTARG
	;;
	s)
		SWRV=$OPTARG
	;;
	f)
		BOARD_CONFIG=$OPTARG
	;;
	d)
		IMG_DBG=$OPTARG
		if [ $IMG_DBG != "DEBUG" ]; then
			usage "Invalid debug Option $IMG_DBG"
			exit 1
		fi
	;;
	u)
		DEBUG_UID=$OPTARG
	;;
	j)
		DEBUG_TYPE=$OPTARG
		dbg_valid=0
		cnt=1
		for tdbg in $VALID_DBG_TYPES
		do
			if [ "$tdbg" == "$DEBUG_TYPE" ]; then
				dbg_valid=$cnt
			fi
			cnt=$(($cnt * 2))
		done
		if [ $dbg_valid == 0 ]; then
			usage "Invalid debug_type Option $DEBUG_TYPE"
			exit 1
		else
			DEVICE_DEBUG_TYPE=`expr $dbg_valid / 2`
		fi
	;;
	y)
		IMG_ENC=$OPTARG
		enc_valid=0
		for tenc in $VALID_ENC
		do
			if [ "$tenc" == "$IMG_ENC" ]; then
				enc_valid=1
			fi
		done
		if [ $enc_valid == 0 ]; then
			usage "Invalid Encrypt Option $IMG_ENC"
			exit 1
		fi
	;;
	c)
		CERT_SIGN=$OPTARG
		cert_valid=0
		for tcert in $VALID_CERT
		do
			if [ "$tcert" == "$CERT_SIGN" ]; then
				cert_valid=1
			fi
		done
		if [ $cert_valid == 0 ]; then
			usage "Invalid Cert type $CERT_SIGN"
			exit 1
		fi
	;;
	m)
		if [ $OPTARG == "SPLIT_MODE" ]; then
			R5_BOOTCORE_OPTS=2
		fi
	;;
	a)
		SBL_IMAGE_TYPE=$OPTARG
	;;
	w)
		SYSFW=$OPTARG
	;;
	v)
		SYSFW_LOADADDR=$OPTARG
	;;
	g)
		SYSFW_DATA=$OPTARG
	;;
	n)
		SYSFW_DATA_LOADADDR=$OPTARG
	;;
	t)
		DM_DATA=$OPTARG
	;;
	p)
		DM_DATA_LOADADDR=$OPTARG
	;;
	q)
		SYSFW_INNER_CERT=$OPTARG
	;;
	h)
		usage
		exit 0
	;;
	\?)
		usage "Invalid Option '-$OPTARG'"
		exit 1
	;;
	:)
		usage "Option '-$OPTARG' Needs an argument."
		exit 1
	;;
	esac
done

if [ "$#" -eq 0 ]
then
	usage "Arguments missing"
	exit 1
fi

if [ -z "$BIN" ]; then
	usage "Input bin file missing"
	exit 1
fi

if [[ $SBL_IMAGE_TYPE == "combined" ]]; then
	echo "Combined SBL image will be created"
	if [ -z "$BIN" -o -z "$LOADADDR" ]; then
		usage "Bootloader and its loadaddr are compulsory"
		exit 1
	fi

	if [ -z "$SYSFW" -o -z "$SYSFW_LOADADDR" ]; then
		usage "SYSFW and its loadaddr are compulsory"
		exit 1
	fi

	if [ -z "$SYSFW_DATA" -o -z "$SYSFW_DATA_LOADADDR" ]; then
		usage "SYSFW board configuration and its loadaddr are compulsory"
		exit 1
	fi
fi

# Create a temp workarea based on input file. This allows the script
# to be invoked simultaneously for different appimages when parallel
# make is used.
WORK_DIR="${BIN%.*}"
mkdir -p $WORK_DIR
pushd .
cd  $WORK_DIR

#Generate random key if user doesn't provide a key.
if [ -z "$SIGN_KEY" ]; then
if [[ $SBL_IMAGE_TYPE == "combined" ]]; then
	gen_ecdsakey
else
	gen_rsakey
fi
fi

if [ ! -z $IMG_DBG ]; then
	echo
	echo "Debug Extension is Enabled :"
	if [ -z $DEBUG_UID ]; then
		echo "SoC UID not specified for Debug Extension. Using UID 0"
	else
		DEVICE_UID=`cat $DEBUG_UID`
	fi
	echo "	UID = $DEVICE_UID"
	echo "	DBG_TYPE = $DEVICE_DEBUG_TYPE"
fi

#if Image encryption is enabled, encrypt the binary
if [ ! -z $IMG_ENC ]; then
	if [ "$CERT_SIGN" == "DMSC_O" ];then
		echo "DMSC FW Inner certificate enryption not Supported for DMSC Outer Certificate :"
		exit 1
	else
		image_encrypt
		BIN=$ENC_BIN
	fi
fi


if [ "$CERT_SIGN" == "R5" ];then
	BOOTCORE_ID=16
	CERT_TYPE=1
	BOOTCORE_OPTS=$R5_BOOTCORE_OPTS
else
	BOOTCORE_ID=0
	BOOTCORE_OPTS=0
	if [ "$CERT_SIGN" == "DMSC_O" ];then
		echo "Payload should be DMSC FW Inner certificate "
		CERT_TYPE=3
	else
		CERT_TYPE=2
	fi
fi

#Create CERT name
CERT=$CERT_SIGN"-cert.bin"

#Compute SHA512 of the Binary
SHA_VAL=`openssl dgst -$SHA -hex $BIN | sed -e "s/^.*= //g"`

#Get the Binary Size
BIN_SIZE=`cat $BIN | wc -c | tr -d ' '`
ADDR=`printf "%08x" $LOADADDR`

if [[ $SBL_IMAGE_TYPE == "combined" ]]; then
	SHA_OID=${sha_oids["$SHA"]}
	SBL_SHA_VAL=`openssl dgst -$SHA -hex $BIN | sed -e "s/^.*= //g"`
	SBL_SIZE=`cat $BIN | wc -c`
	SBL_ADDR=`printf "%08x" $LOADADDR`

	SYSFW_SHA_VAL=`openssl dgst -$SHA -hex $SYSFW | sed -e "s/^.*= //g"`
	SYSFW_SIZE=`cat $SYSFW | wc -c`
	SYSFW_ADDR=`printf "%08x" $SYSFW_LOADADDR`

	SYSFW_DATA_SHA_VAL=`openssl dgst -$SHA -hex $SYSFW_DATA | sed -e "s/^.*= //g"`
	SYSFW_DATA_SIZE=`cat $SYSFW_DATA | wc -c`
	SYSFW_DATA_ADDR=`printf "%08x" $SYSFW_DATA_LOADADDR`

	NUM_COMPS_COUNT=3

	if [ -n "$SYSFW_INNER_CERT" ]; then
       SYSFW_INNER_CERT_SHA_VAL=`openssl dgst -$SHA -hex $SYSFW_INNER_CERT | sed -e "s/^.*= //g"`
       SYSFW_INNER_CERT_SIZE=`cat $SYSFW_INNER_CERT | wc -c`
       NUM_COMPS_COUNT=$(expr $NUM_COMPS_COUNT + 1)
       SYSFW_INNER_CERT_EXT_BOOT_SEQUENCE_STRING="sysfw_inner_cert=SEQUENCE:sysfw_inner_cert"
	read -r -d '' SYSFW_INNER_CERT_EXT_BOOT_BLOCK << EOM
	\\
	[sysfw_inner_cert]\\
	compType = INTEGER:3\\
	bootCore = INTEGER:0\\
	compOpts = INTEGER:0\\
	destAddr = FORMAT:HEX,OCT:00000000\\
	compSize = INTEGER:SYSFW_INNER_CERT_IMAGE_SIZE\\
	shaType  = OID:SYSFW_INNER_CERT_SHA_OID\\
	shaValue = FORMAT:HEX,OCT:SYSFW_INNER_CERT_SHA_VAL
EOM
	else
       SYSFW_INNER_CERT_SIZE=`printf "%08x" 0`
       SYSFW_INNER_CERT_EXT_BOOT_SEQUENCE_STRING=""
       SYSFW_INNER_CERT_EXT_BOOT_BLOCK=""
	fi

	if [ -n "$DM_DATA" ]; then
		DM_DATA_SHA_VAL=`openssl dgst -$SHA -hex $DM_DATA | sed -e "s/^.*= //g"`
		DM_DATA_SIZE=`cat $DM_DATA | wc -c`
		DM_DATA_ADDR=`printf "%08x" $DM_DATA_LOADADDR`
		NUM_COMPS_COUNT=$(expr $NUM_COMPS_COUNT + 1)
		DM_DATA_EXT_BOOT_SEQUENCE_STRING="dm_data=SEQUENCE:dm_data"
	read -r -d '' DM_DATA_EXT_BOOT_BLOCK << EOM
	\\
	[dm_data]\\
	compType = INTEGER:17\\
	bootCore = INTEGER:16\\
	compOpts = INTEGER:0\\
	destAddr = FORMAT:HEX,OCT:DM_DATA_DEST_ADDR\\
	compSize = INTEGER:DM_DATA_IMAGE_SIZE\\
	shaType  = OID:DM_DATA_IMAGE_SHA_OID\\
	shaValue = FORMAT:HEX,OCT:DM_DATA_IMAGE_SHA_VAL
EOM
	else
		DM_DATA_SIZE=`printf "%08x" 0`
		DM_DATA_EXT_BOOT_SEQUENCE_STRING=""
		DM_DATA_EXT_BOOT_BLOCK=""
	fi

	TOTAL_SIZE=$(expr $BIN_SIZE + $SYSFW_SIZE + $SYSFW_DATA_SIZE + $SYSFW_INNER_CERT_SIZE + $DM_DATA_SIZE)
fi

gen_cert() {
if [[ $BOARD_CONFIG == 0 ]]; then
	echo "$CERT_SIGN Certificate being generated :"
	echo "	X509_CFG = $TEMP_X509"
	echo "	KEY = $SIGN_KEY"
	echo "	BIN = $BIN"
	echo "	CERT TYPE = $CERT_SIGN, $CERT_TYPE"
	echo "	CORE ID = $BOOTCORE_ID"
	echo "	LOADADDR = 0x$ADDR"
	echo "	IMAGE_SIZE = $BIN_SIZE"
	echo "	BOOT_OPTIONS = $BOOTCORE_OPTS"
fi
	echo "Certificate being generated :"
if [[ $SBL_IMAGE_TYPE == "combined" ]]; then
	sed -i "s/SW_REV/$SWRV/"  $X509_DEFAULT
	#echo $SBL_ADDR $SBL_SIZE $SBL_SHA_VAL
	sed -i "s/NUM_COMPS_COUNT/$NUM_COMPS_COUNT/"  $X509_DEFAULT
	sed -i "s/BOOTCORE_OPTS/$BOOTCORE_OPTS/"  $X509_DEFAULT
	sed -i "s/SBL_DEST_ADDR/$SBL_ADDR/"  $X509_DEFAULT
	sed -i "s/SBL_IMAGE_SIZE/$SBL_SIZE/" $X509_DEFAULT
	sed -i "s/SBL_IMAGE_SHA_OID/$SHA_OID/" $X509_DEFAULT
	sed -i "s/SBL_IMAGE_SHA_VAL/$SBL_SHA_VAL/" $X509_DEFAULT
	#echo $SYSFW_ADDR $SYSFW_SIZE $SYSFW_SHA_VAL
	sed -i "s/SYSFW_DEST_ADDR/$SYSFW_ADDR/" $X509_DEFAULT
	sed -i "s/SYSFW_IMAGE_SIZE/$SYSFW_SIZE/" $X509_DEFAULT
	sed -i "s/SYSFW_IMAGE_SHA_OID/$SHA_OID/" $X509_DEFAULT
	sed -i "s/SYSFW_IMAGE_SHA_VAL/$SYSFW_SHA_VAL/" $X509_DEFAULT
	#echo $SYSFW_DATA_ADDR $SYSFW_DATA_SIZE $SYSFW_DATA_SHA_VAL
	sed -i "s/SYSFW_DATA_DEST_ADDR/$SYSFW_DATA_ADDR/" $X509_DEFAULT
	sed -i "s/SYSFW_DATA_IMAGE_SIZE/$SYSFW_DATA_SIZE/" $X509_DEFAULT
	sed -i "s/SYSFW_DATA_IMAGE_SHA_OID/$SHA_OID/" $X509_DEFAULT
	sed -i "s/SYSFW_DATA_IMAGE_SHA_VAL/$SYSFW_DATA_SHA_VAL/" $X509_DEFAULT
	#echo $SYSFW_INNER_CERT_ADDR $SYSFW_INNER_CERT_SIZE $SYSFW_INNER_CERT_VAL
	sed -i "s/SYSFW_INNER_CERT_EXT_BOOT_BLOCK/$SYSFW_INNER_CERT_EXT_BOOT_BLOCK/" $X509_DEFAULT
	sed -i "s/SYSFW_INNER_CERT_EXT_BOOT_SEQUENCE_STRING/$SYSFW_INNER_CERT_EXT_BOOT_SEQUENCE_STRING/" $X509_DEFAULT
	sed -i "s/SYSFW_INNER_CERT_DEST_ADDR/$SYSFW_INNER_CERT_ADDR/" $X509_DEFAULT
	sed -i "s/SYSFW_INNER_CERT_IMAGE_SIZE/$SYSFW_INNER_CERT_SIZE/" $X509_DEFAULT
	sed -i "s/SYSFW_INNER_CERT_SHA_OID/$SHA_OID/" $X509_DEFAULT
	sed -i "s/SYSFW_INNER_CERT_SHA_VAL/$SYSFW_INNER_CERT_SHA_VAL/" $X509_DEFAULT
	#echo $DM_DATA_ADDR $DM_DATA_SIZE $DM_DATA_SHA_VAL
	sed -i "s/DM_DATA_EXT_BOOT_BLOCK/$DM_DATA_EXT_BOOT_BLOCK/" $X509_DEFAULT
	sed -i "s/DM_DATA_EXT_BOOT_SEQUENCE_STRING/$DM_DATA_EXT_BOOT_SEQUENCE_STRING/" $X509_DEFAULT
	sed -i "s/DM_DATA_DEST_ADDR/$DM_DATA_ADDR/" $X509_DEFAULT
	sed -i "s/DM_DATA_IMAGE_SIZE/$DM_DATA_SIZE/" $X509_DEFAULT
	sed -i "s/DM_DATA_IMAGE_SHA_OID/$SHA_OID/" $X509_DEFAULT
	sed -i "s/DM_DATA_IMAGE_SHA_VAL/$DM_DATA_SHA_VAL/" $X509_DEFAULT
	#echo $DEVICE_UID $DEVICE_DEBUG_TYPE
	sed -i "s/TEST_DEVICE_UID/$DEVICE_UID/" $X509_DEFAULT
    sed -i "s/TEST_DEVICE_DEBUG_TYPE/$DEVICE_DEBUG_TYPE/" $X509_DEFAULT
	#echo $TOTAL_SIZE
	sed -i "s/TOTAL_IMAGE_LENGTH/$TOTAL_SIZE/" $X509_DEFAULT
	openssl req -new -x509 -key $SIGN_KEY -nodes -outform DER -out $CERT -config $X509_DEFAULT -$SHA
	else
	sed -e "s/TEST_IMAGE_LENGTH/$BIN_SIZE/"	\
		-e "s/TEST_IMAGE_SHA512/$SHA_VAL/" \
		-e "s/TEST_SWRV/$SWRV/" \
		-e "s/TEST_CERT_TYPE/$CERT_TYPE/" \
		-e "s/TEST_BOOT_CORE_ID/$BOOTCORE_ID/" \
		-e "s/TEST_BOOT_CORE_OPTS/$BOOTCORE_OPTS/" \
		-e "s/TEST_IMAGE_ENC_IV/$ENC_IV_VAL/" \
		-e "s/TEST_IMAGE_ENC_RS/$ENC_RS_VAL/" \
		-e "s/TEST_IMAGE_KEY_DERIVE_INDEX/$ENC_KD_INDEX_VAL/" \
		-e "s/TEST_IMAGE_KEY_DERIVE_SALT/$ENC_KD_SALT_VAL/" \
		-e "s/TEST_DEVICE_UID/$DEVICE_UID/" \
		-e "s/TEST_DEVICE_DEBUG_TYPE/$DEVICE_DEBUG_TYPE/" \
		-e "s/TEST_BOOT_ADDR/$ADDR/" ${X509_DEFAULT} > $TEMP_X509
	openssl req -new -x509 -key $SIGN_KEY -nodes -outform DER -out $CERT -config $TEMP_X509 -$SHA
	fi
}

gen_x509template() {
cat << __HEADER_EOF > $X509_DEFAULT
 [ req ]
 distinguished_name     = req_distinguished_name
 x509_extensions        = v3_ca
 prompt                 = no

 dirstring_type = nobmp

 [ req_distinguished_name ]
 C                      = US
 ST                     = SC
 L                      = Dallas
 O                      = Texas Instruments., Inc.
 OU                     = PBU
 CN                     = Albert
 emailAddress           = Albert@ti.com

 [ v3_ca ]
  basicConstraints = CA:true
  1.3.6.1.4.1.294.1.3=ASN1:SEQUENCE:swrv
__HEADER_EOF

if [[ $SBL_IMAGE_TYPE != "combined" ]]; then
	cat << __HEADER_SYSFW_EOF >> $X509_DEFAULT
  1.3.6.1.4.1.294.1.34=ASN1:SEQUENCE:sysfw_image_integrity
  1.3.6.1.4.1.294.1.35=ASN1:SEQUENCE:sysfw_image_load
__HEADER_SYSFW_EOF
else
cat << __HEADER_SYSFW_EOF >> $X509_DEFAULT
  1.3.6.1.4.1.294.1.9=ASN1:SEQUENCE:ext_boot_info
__HEADER_SYSFW_EOF
fi

if [ ! -z $IMG_ENC ]; then
	cat << __HEADER_ENC_EOF >> $X509_DEFAULT
  1.3.6.1.4.1.294.1.4=ASN1:SEQUENCE:encryption
__HEADER_ENC_EOF
fi


if [[ $SBL_IMAGE_TYPE != "combined" ]]; then
if [[ $BOARD_CONFIG == 0 ]]; then
	cat << __HEADER_ENC_EOF >> $X509_DEFAULT
  1.3.6.1.4.1.294.1.1=ASN1:SEQUENCE:boot_seq
  1.3.6.1.4.1.294.1.2=ASN1:SEQUENCE:image_integrity
__HEADER_ENC_EOF
fi
fi

if [ ! -z $IMG_DBG ]; then
	cat << __HEADER_DBG_EOF >> $X509_DEFAULT
  1.3.6.1.4.1.294.1.8=ASN1:SEQUENCE:debug
__HEADER_DBG_EOF
fi

if [[ $SBL_IMAGE_TYPE != "combined" ]]; then
cat << __IMAGE_DEFAULT_EOF >> $X509_DEFAULT

 [ swrv ]
  swrv         =  INTEGER:TEST_SWRV

 [ sysfw_image_integrity ]
  shaType = OID:2.16.840.1.101.3.4.2.3
  shaValue = FORMAT:HEX,OCT:TEST_IMAGE_SHA512
  imageSize = INTEGER:TEST_IMAGE_LENGTH

__IMAGE_DEFAULT_EOF

if [[ $BOARD_CONFIG == 0 ]]; then
	cat << __IMAGE_ENC_EOF >> $X509_DEFAULT
 [ boot_seq ]
  certType     =  INTEGER:TEST_CERT_TYPE
  bootCore     =  INTEGER:TEST_BOOT_CORE_ID
  bootCoreOpts =  INTEGER:TEST_BOOT_CORE_OPTS
  destAddr     =  FORMAT:HEX,OCT:TEST_BOOT_ADDR
  imageSize    =  INTEGER:TEST_IMAGE_LENGTH

 [ image_integrity ]
  shaType      =  OID:2.16.840.1.101.3.4.2.3
  shaValue     =  FORMAT:HEX,OCT:TEST_IMAGE_SHA512

 [ sysfw_image_load ]
  destAddr = FORMAT:HEX,OCT:TEST_BOOT_ADDR
  authInPlace = INTEGER:1
__IMAGE_ENC_EOF
else
	cat << __IMAGE_ENC_EOF >> $X509_DEFAULT
 [ sysfw_image_load ]
  destAddr = FORMAT:HEX,OCT:00000000
  authInPlace = INTEGER:2
__IMAGE_ENC_EOF
fi
else
	cat << __IMAGE_COMBINE_EOF >> $X509_DEFAULT
 [swrv]
 swrv=INTEGER:SW_REV

 [ext_boot_info]
 extImgSize=INTEGER:TOTAL_IMAGE_LENGTH
 numComp=INTEGER:NUM_COMPS_COUNT
 sbl=SEQUENCE:sbl
 sysfw=SEQUENCE:sysfw
 sysfw_data=SEQUENCE:sysfw_data
 SYSFW_INNER_CERT_EXT_BOOT_SEQUENCE_STRING
 DM_DATA_EXT_BOOT_SEQUENCE_STRING

 [sbl]
 compType = INTEGER:1
 bootCore = INTEGER:16
 compOpts = INTEGER:BOOTCORE_OPTS
 destAddr = FORMAT:HEX,OCT:SBL_DEST_ADDR
 compSize = INTEGER:SBL_IMAGE_SIZE
 shaType  = OID:SBL_IMAGE_SHA_OID
 shaValue = FORMAT:HEX,OCT:SBL_IMAGE_SHA_VAL

 [sysfw]
 compType = INTEGER:2
 bootCore = INTEGER:0
 compOpts = INTEGER:0
 destAddr = FORMAT:HEX,OCT:SYSFW_DEST_ADDR
 compSize = INTEGER:SYSFW_IMAGE_SIZE
 shaType  = OID:SYSFW_IMAGE_SHA_OID
 shaValue = FORMAT:HEX,OCT:SYSFW_IMAGE_SHA_VAL

 [sysfw_data]
 compType = INTEGER:18
 bootCore = INTEGER:0
 compOpts = INTEGER:0
 destAddr = FORMAT:HEX,OCT:SYSFW_DATA_DEST_ADDR
 compSize = INTEGER:SYSFW_DATA_IMAGE_SIZE
 shaType  = OID:SYSFW_DATA_IMAGE_SHA_OID
 shaValue = FORMAT:HEX,OCT:SYSFW_DATA_IMAGE_SHA_VAL
 SYSFW_INNER_CERT_EXT_BOOT_BLOCK
 DM_DATA_EXT_BOOT_BLOCK
__IMAGE_COMBINE_EOF
fi

if [ ! -z $IMG_ENC ]; then
	cat << __IMAGE_ENC_EOF >> $X509_DEFAULT

 [ encryption ]
  initalVector =  FORMAT:HEX,OCT:TEST_IMAGE_ENC_IV
  randomString =  FORMAT:HEX,OCT:TEST_IMAGE_ENC_RS
  iterationCnt =  INTEGER:TEST_IMAGE_KEY_DERIVE_INDEX
  salt         =  FORMAT:HEX,OCT:TEST_IMAGE_KEY_DERIVE_SALT
__IMAGE_ENC_EOF
fi

if [ ! -z $IMG_DBG ]; then
	cat << __IMAGE_DBG_EOF >> $X509_DEFAULT

 [ debug ]
  debugUID     =  FORMAT:HEX,OCT:TEST_DEVICE_UID
  debugType    =  INTEGER:TEST_DEVICE_DEBUG_TYPE
  coreDbgEn    =  INTEGER:0
  coreDbgSecEn =  INTEGER:0
__IMAGE_DBG_EOF
fi
}


gen_x509template
gen_cert

if [[ $SBL_IMAGE_TYPE == "combined" ]]; then
	cat $CERT $BIN $SYSFW $SYSFW_DATA $SYSFW_INNER_CERT $DM_DATA > $OUTPUT
else
cat $CERT $BIN > $OUTPUT
fi

if [[ $BOARD_CONFIG == 0 ]]; then
echo "SUCCESS: Image $OUTPUT generated. Good to boot"
else
echo "SUCCESS: Image $OUTPUT generated."
fi


#Remove all intermediate files
rm -f $X509_DEFAULT
rm -f enc*.bin enciv.txt encrs.txt encsalt.txt encprk.txt encgenkey.txt 1.bin

#uncomment below to remove local x509 template used to generate certificate
rm -f $TEMP_X509
rm -f $CERT

popd > /dev/null
rm -rf $WORK_DIR
#intentionally not removing *.pem, enckey.txt, *-cert.bin, *ENC*.bin
