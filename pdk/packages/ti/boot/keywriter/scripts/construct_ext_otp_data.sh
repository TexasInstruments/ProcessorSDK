#!/bin/bash

EXT_OTP_DATA="0xFFFFFFFF"
# Start index for ext_otp (offset into 1024 bit eFuse array)
INDEX=0
# Size of the ext_otp_data in bits
SIZE=25

######################### PARSE #################################
HELP_TXT="
$0 - Construct extended otp data
    -extotp
        Hexadecimal number
        Eg: 0xABCDEFG
		Defaults to 0xFFFF FFFF
    -indx
        Start index (in bits) to be offset into 1024 bit eFuse Array
		Defaults to 0
		* Needs to be multiple of 8
    -size
        Size of the data (in bits)
		Defaults to 1024
		* Needs to be multiple of 8
    Example Usage:
    $0 -extotp 0xC0FFEE -indx 8 -size 24
"

# HELP TEXT PRINT
if [ "$#" -lt 1 ]; then
	echo "$HELP_TXT"
	exit
fi

while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -extotp)
	EXT_OTP_DATA="$2"
    shift # past argument
    shift # past value
    ;;
	-indx)
	INDEX="$2"
	if [[ $INDEX -gt 1016 ]]; then
		echo "ERR: Max allowed indx is 1024 - 8 = 1016"
		exit
	fi
	shift # past argument
    shift # past value
    ;;
	-size)
	SIZE="$2"
	if [[ $SIZE -gt 1024 ]]; then
		echo "ERR: Max allowed size is 1024"
		exit
	fi
	shift # past argument
    shift # past value
    ;;
	*)
		echo "ERR: Invalid input argument(s)"
		echo "$1" "$2"
		exit
	shift
	shift
	;;
esac
done
#################################################################

if [[ $(( $INDEX + $SIZE )) -gt 1024 ]]; then
	echo "ERR: INDEX + SIZE can't exceed 1024"
	exit
fi

if [ $((INDEX%8)) -ne 0 ]; then
	echo "ERR: INDEX not multiple of 8"
	exit
fi

if [ $((SIZE%8)) -ne 0 ]; then
	echo "ERR: SIZE not multiple of 8"
	exit
fi


# delete all matches of the string 0x
EXT_OTP_DATA=${EXT_OTP_DATA//0x/}

# check for hexadecimal number
if [[ ! "${EXT_OTP_DATA}" =~ ^[0-9a-fA-F]+$ ]]; then
	echo "ERR: EXT_OTP_DATA is not a hexadecimal number!!"
	exit
fi

if [ $SIZE -lt $((${#EXT_OTP_DATA}*4)) ]; then
	echo "WARNING: Size less than ext_otp_data input"
	echo "Size: $SIZE"
	echo "Input size: $((${#EXT_OTP_DATA}*4))"
	exit
else
	# Add MSB 0's to match SIZE
	while [ $SIZE -gt $((${#EXT_OTP_DATA}*4)) ];
	do
		EXT_OTP_DATA="0$EXT_OTP_DATA"
	done
fi

leading_zro_octets=$(( $INDEX/4 ))
otp_octets=$(( $SIZE/4 ))
zro_octets=$((256 - $leading_zro_octets - $otp_octets))

if [ $INDEX -gt 0 ]; then
	FINAL_DATA_BASE16=$(printf "%0${leading_zro_octets}x%s%0${zro_octets}x" 0 $EXT_OTP_DATA 0)
else
	FINAL_DATA_BASE16=$(printf "%s%0${zro_octets}x" $EXT_OTP_DATA 0)
fi

echo $FINAL_DATA_BASE16 > ext_otp_data.txt
xxd -p -r ext_otp_data.txt > ext_otp_data.bin
# Print to STDOUT and text file
echo "ext_otp[0:1023]"
xxd -p -c 16 ext_otp_data.bin | tee ext_otp_data.txt
du -b ext_otp_data.bin
ls ext_otp_data.txt
# rm ext_otp_data.txt
