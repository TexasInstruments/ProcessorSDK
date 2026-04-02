#!/bin/bash

######################################################################################
# UTILITY FUNCTIONS.
######################################################################################

# parse_validate_mek_opt <raw_mek_opt_string>
parse_validate_mek_opt(){
	MEK_OPT_SIZE=5
	LIMIT_BITS_MASK=$(( ( 1 << ${MEK_OPT_SIZE} ) - 1 ))
	mek_opt_info[flag]="yes"
	# delete all matches of the string 0x
	mek_opt_info[val]=${1//0x/}

	# check for hexadecimal number
	if [[ ! "${mek_opt_info[val]}" =~ ^[0-9a-fA-F]+$ ]]; then
		echo "ERR: MEK_OPT is not a hexadecimal number!!"
		exit
	fi

	# check if the size > 2 octets (for 8 bits)
	if [ "${#mek_opt_info[val]}" -gt 2 ]; then
		echo "ERR: MEK_OPT provided is too large (> 8 bits)"
		exit
	fi
	val_base10=$((16#${mek_opt_info[val]}))
	final_data_base10=$(( $val_base10 & $LIMIT_BITS_MASK ))

	mek_opt_info[val]=$(printf "%02x" $final_data_base10)

	echo "# Using MEK_OPTIONS[4:0]: 0x${mek_opt_info[val]}" 
}

# parse_validate_mpk_opt <raw_mpk_opt_string>
parse_validate_mpk_opt(){
	MPK_OPT_SIZE=10
	LIMIT_BITS_MASK=$(( ( 1 << ${MPK_OPT_SIZE} ) - 1 ))
	mpk_opt_info[flag]="yes"
	# delete all matches of the string 0x
	mpk_opt_info[val]=${1//0x/}

	# check for hexadecimal number
	if [[ ! "${mpk_opt_info[val]}" =~ ^[0-9a-fA-F]+$ ]]; then
		echo "ERR: MPK_OPT is not a hexadecimal number!!"
		exit
	fi

	# check if the size > 3 octets (for 10 bits)
	if [ "${#mpk_opt_info[val]}" -gt 3 ]; then
		echo "ERR: MPK_OPT provided is too large (> 12 bits)"
		exit
	fi
	val_base10=$((16#${mpk_opt_info[val]}))
	final_data_base10=$(( $val_base10 & $LIMIT_BITS_MASK ))

	# Openssl gives error for odd number of hex string
	mpk_opt_info[val]=$(printf "%04x" $final_data_base10)

	echo "# Using MPK_OPTIONS[9:0]: 0x${mpk_opt_info[val]}" 
}

# parse_validate_msv <raw_msv_string>
parse_validate_msv(){
	msv_info[flag]="yes"
	# delete all matches of the string 0x
	msv_info[val]=${1//0x/}

	# check for hexadecimal number
	if [[ ! "${msv_info[val]}" =~ ^[0-9a-fA-F]+$ ]]; then
		echo "ERR: MSV is not a hexadecimal number!!"
		exit
	fi

	# check if the size > 5 octets (20 bits)
	if [ "${#msv_info[val]}" -gt 5 ]; then
		echo "ERR: MSV provided is too large (> 20 bits)"
		exit
	fi

	# prepend zeros
	while [ "${#msv_info[val]}" -ne 8 ];do 
		msv_info[val]="0"${msv_info[val]}
	done
	
	echo "# Using MSV[19:0]: 0x${msv_info[val]}" 
}

# parse_validate_jtag_disable <raw_key_rev_string>
parse_validate_jtag_disable(){
	jtag_disable_info[flag]="yes"
	jtag_disable_info[val]="F"
	
	# prepend zeros
	while [ "${#jtag_disable_info[val]}" -ne 8 ];do 
		jtag_disable_info[val]="0"${jtag_disable_info[val]}
	done

	echo "# Using Jtag disable: 0x${jtag_disable_info[val]}" 
}

# parse_validate_key_cnt <raw_key_cnt_string>
parse_validate_key_cnt(){
	key_cnt_info[flag]="yes"
	# delete all matches of the string 0x
	key_cnt_info[val]=${1//0x/}

	case ${key_cnt_info[val]} in 
		1|01)
			key_cnt_info[val]="01"
		;;
		2|02)
			# 0x11 is needed for keycount 2
			key_cnt_info[val]="03"
		;;
		*)
			echo "ERR: Key Count should be 1 or 2"
			exit
		;;
	esac	
	
	# prepend zeros
	while [ "${#key_cnt_info[val]}" -ne 8 ];do 
		key_cnt_info[val]="0"${key_cnt_info[val]}
	done

	echo "# Using Key Count: 0x${key_cnt_info[val]}" 
}

# parse_validate_key_rev <raw_key_rev_string>
parse_validate_key_rev(){
	key_rev_info[flag]="yes"
	# delete all matches of the string 0x
	key_rev_info[val]=${1//0x/}

	case ${key_rev_info[val]} in 
		1|01)
			key_rev_info[val]="01"
		;;
		2|02)
			# 0x11 is needed for key rev 2
			key_rev_info[val]="03"
			echo "WARNING: Key Revision is not equal to 1"
		;;
		*)
			echo "ERR: Key Rev should be 1 or 2"
			exit
		;;
	esac	
	
	# prepend zeros
	while [ "${#key_rev_info[val]}" -ne 8 ];do 
		key_rev_info[val]="0"${key_rev_info[val]}
	done

	echo "# Using Key Rev: 0x${key_rev_info[val]}" 
}

# parse_validate_swrev_sysfw <raw_swrev_sysfw_string>
parse_validate_swrev_sysfw(){
	swrev_sysfw_info[flag]="yes"
	swrev_sysfw_info[val]="$1"

	# Max allowed value is 48
	if [ "${swrev_sysfw_info[val]}" -gt 48 ]; then
		echo "ERR: SWREV_SYSFW provided is too large (> 48)"
		exit
	fi

	swrev_sysfw_info[val]=$(( ( 1 << ${swrev_sysfw_info[val]} ) - 1 ))

	swrev_sysfw_info[val]=$(printf "%012x" ${swrev_sysfw_info[val]})

	echo "# Using SWREV_SYSFW: 0x${swrev_sysfw_info[val]}"
}

# parse_validate_swrev_sbl <raw_swrev_sbl_string>
parse_validate_swrev_sbl(){
	swrev_sbl_info[flag]="yes"
	swrev_sbl_info[val]="$1"

	# Max allowed value is 48
	if [ "${swrev_sbl_info[val]}" -gt 48 ]; then
		echo "ERR: SWREV_SBL provided is too large (> 48)"
		exit
	fi

	swrev_sbl_info[val]=$(( ( 1 << ${swrev_sbl_info[val]} ) - 1 ))

	swrev_sbl_info[val]=$(printf "%012x" ${swrev_sbl_info[val]})

	echo "# Using SWREV_SBL: 0x${swrev_sbl_info[val]}"
}

# parse_validate_swrev_sec_brdcfg <raw_swrev_sec_brdcfg_string>
parse_validate_swrev_sec_brdcfg(){
	swrev_sec_brdcfg_info[flag]="yes"
	swrev_sec_brdcfg_info[val]="$1"

	# Max allowed value is 64
	if [ "${swrev_sec_brdcfg_info[val]}" -gt 64 ]; then
		echo "ERR: SWREV_SEC_BRDCFG provided is too large (> 64)"
		exit
	fi

	# Bash can handle only 64 bits of data
	if [ "${swrev_sec_brdcfg_info[val]}" -eq 64 ]; then
		swrev_sec_brdcfg_info[val]=-1
	else
		swrev_sec_brdcfg_info[val]=$(( ( 1 << ${swrev_sec_brdcfg_info[val]} ) - 1 ))
	fi
	swrev_sec_brdcfg_info[val]=$(printf "%016x" ${swrev_sec_brdcfg_info[val]})
	
	echo "# Using SWREV_SEC_BRDCFG: 0x${swrev_sec_brdcfg_info[val]}"
}
# parse_validate_extotp_wprp <raw_wp_rp_string>
parse_validate_extotp_wprp(){
	# delete all matches of the string 0x
	extotp_info[wprp]=${1//0x/}

	# prepend zeros (for 128 bits)
	while [ "${#extotp_info[wprp]}" -ne 32 ];do 
		extotp_info[wprp]="0"${extotp_info[wprp]}
	done

	echo "# Using EXT OTP WP_RP: 0x${extotp_info[wprp]}"
}
# raw_encrypt <PUBLIC-KEY.PEM> <DATA> <ENCRYPTED-OUTPUT>
raw_encrypt(){
	openssl pkeyutl -encrypt -raw -inkey "$1" -pubin -in "$2" -out "$3"
}

# encrypt <PUBLIC-KEY.PEM> <DATA> <ENCRYPTED-OUTPUT>
encrypt(){
	openssl pkeyutl -encrypt -inkey "$1" -pubin -in "$2" -out "$3"
}

# sign_the_hash <PRIV-KEY.PEM> <OUTPUT> <INPUT>
sign_the_hash(){
	openssl dgst -sha512 -sign "$1" -out "$2" "$3"
}

# calc_hash <INPUT> <OUTPUT>
calc_hash(){
	openssl dgst -sha512 -binary "$1" > "$2"
}

# priv_pem_to_pub_der <INPUT> <OUTPUT>
priv_pem_to_pub_der(){
	openssl pkey -in "$1" -pubout -outform DER > "$2"
}

# gen_iv <OUTFILE>
gen_iv(){
	openssl rand -out "$1" 16
}

# gen_rs <OUTFILE>
gen_rs(){
	openssl rand -out "$1" 32
}

# gen_pub_key_x509_extension <PRIV.PEM> <PUB.DER> <PUB.HASH> <SMPKH/BMPKH> <IV> <RS> <FIELD> <OUTPUT>
gen_pub_key_x509_extension(){
	# priv_pem_to_pub_der <INPUT> <OUTPUT>
	priv_pem_to_pub_der "$1" "$2"
	# calc_hash <INPUT> <OUTPUT>
	calc_hash "$2" "$3"
	echo "$4,$(xxd -p $3)" >> "${output_info[hash_csv]}"
	# gen_iv <OUTFILE>
	gen_iv "$5"
	# gen_rs <OUTFILE>
	gen_rs "$6"
	
	IV=$(xxd -p -c 32 "$5")
	cat "$3" "$6" > "$7"
	openssl aes-256-cbc -e -K "${aes256key_info[val]}" -iv "${IV}" -nopad -in "$7" \
			-out "$8"
}

# gen_sym_key_x509_extension <SMEKH/BMEKH> <SYMKEY> <SYMKEY.HASH> <IV> <RS> <FIELD> <OUTPUT>
gen_sym_key_x509_extension(){
	# gen_iv <OUTFILE>
	gen_iv "$4"
	# gen_rs <OUTFILE>
	gen_rs "$5"
	IV=$(xxd -p -c 32 "$4")
	cat "$2" "$5" > "$6"
	openssl dgst -sha512 -binary "$2" > "$3"
	echo "$1,$(xxd -p $3)" >> "${output_info[hash_csv]}"
	openssl aes-256-cbc -e -K "${aes256key_info[val]}" -iv "${IV}" -nopad -in "$6" -out "$7"
}

# gen_ext_otp_key_x509_extension <ext_otp_data> <HASH> <IV> <RS> <FIELD> <OUTPUT>
gen_ext_otp_x509_extension(){
	# gen_iv <OUTFILE>
	gen_iv "$3"
	# gen_rs <OUTFILE>
	gen_rs "$4"
	IV=$(xxd -p -c 32 "$3")
	cat "$1" "$4" > "$5"
	openssl dgst -sha512 -binary "$1" > "$2"
	openssl aes-256-cbc -e -K "${aes256key_info[val]}" -iv "${IV}" -nopad -in "$5" -out "$6"
}

string_of_zeros(){
	s_o_zeros=""

	for i in $(seq 1 $1)
	do
		s_o_zeros="0$s_o_zeros"
	done
	echo $s_o_zeros
}

# populate_config_primary
populate_config_primary(){
	ENC_AES_KEY=$(xxd -p -c 1000000 tmpdir/enc_aes_key.enc)
	sed "s/PUT_ENC_AES_KEY/${ENC_AES_KEY}/" templates/config_template.txt > "${primary_cert_info[config]}" 
	sed "s/PUT_SIZE_ENC_AES/$((${#ENC_AES_KEY}/2))/" -i "${primary_cert_info[config]}"

	ENC_SMPK_SIGNED_AES_KEY=$(xxd -p -c 1000000 tmpdir/enc_smpk_signed_aes_key.enc)
	sed -i "s/PUT_ENC_SMPK_SIGNED_AES_KEY/${ENC_SMPK_SIGNED_AES_KEY}/" "${primary_cert_info[config]}" 
	sed "s/PUT_SIZE_ENC_SMPK_SIGNED_AES/$((${#ENC_SMPK_SIGNED_AES_KEY}/2))/" -i "${primary_cert_info[config]}"

	AESENC_SMPKH=$(xxd -p -c 1000000 tmpdir/aesenc_smpkh.enc)
	AESENC_IV_SMPKH=$(xxd -p -c 1000000 tmpdir/smpkh.iv)
	AESENC_RS_SMPKH=$(xxd -p -c 1000000 tmpdir/smpkh.rs)
	sed -i "s/PUT_AESENC_SMPKH/${AESENC_SMPKH}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_IV_AESENC_SMPKH/${AESENC_IV_SMPKH}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_RS_AESENC_SMPKH/${AESENC_RS_SMPKH}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_SIZE_AESENC_SMPKH/$((${#AESENC_SMPKH}/2))/" "${primary_cert_info[config]}"

	[[ ${smpk_info[flag]} == "yes" ]] && active_flag=${ENABLE_VAL} || active_flag=${DISABLE_VAL} 
	[[ ${smpk_info[wp]} == "yes" ]] && wp_flag=${ENABLE_VAL} || wp_flag=${DISABLE_VAL} 
	[[ ${smpk_info[rp]} == "yes" ]] && rp_flag=${ENABLE_VAL} || rp_flag=${DISABLE_VAL} 
	[[ ${smpk_info[ovrd]} == "yes" ]] && ovrd_flag=${ENABLE_VAL} || ovrd_flag=${DISABLE_VAL} 

	# <wp> <rp> <override> <active/inavtive>
	sed -i "s/PUT_ACTFLAG_AESENC_SMPKH/$((16#${wp_flag}${rp_flag}${ovrd_flag}${active_flag}))/" "${primary_cert_info[config]}"

	[[ ${jtag_disable_info[flag]} == "yes" ]] && active_flag=${ENABLE_VAL} || active_flag=${DISABLE_VAL} 
	[[ ${jtag_disable_info[wp]} == "yes" ]] && wp_flag=${ENABLE_VAL} || wp_flag=${DISABLE_VAL} 
	[[ ${jtag_disable_info[rp]} == "yes" ]] && rp_flag=${ENABLE_VAL} || rp_flag=${DISABLE_VAL} 
	[[ ${jtag_disable_info[ovrd]} == "yes" ]] && ovrd_flag=${ENABLE_VAL} || ovrd_flag=${DISABLE_VAL} 
	
	# <wp> <rp> <override> <active/inavtive>
	sed -i "s/PUT_ACTFLAG_JTAG_DISABLE/$((16#${wp_flag}${rp_flag}${ovrd_flag}${active_flag}))/" "${primary_cert_info[config]}"
	sed -i "s/PUT_PLAIN_JTAG_DISABLE/${jtag_disable_info[val]}/" "${primary_cert_info[config]}"

	[[ ${mpk_opt_info[flag]} == "yes" ]] && active_flag=${ENABLE_VAL} || active_flag=${DISABLE_VAL} 
	# <wp> <rp> <override> <active/inavtive>
	sed -i "s/PUT_ACTFLAG_PLAIN_MPK_OPT/$((16#${DISABLE_VAL}${DISABLE_VAL}${DISABLE_VAL}${active_flag}))/" "${primary_cert_info[config]}"
	sed -i "s/PUT_PLAIN_MPK_OPT/${mpk_opt_info[val]}/" "${primary_cert_info[config]}"


	AESENC_SMEK=$(xxd -p -c 1000000 tmpdir/aesenc_smek.enc)
	AESENC_IV_SMEK=$(xxd -p -c 1000000 tmpdir/smek.iv)
	AESENC_RS_SMEK=$(xxd -p -c 1000000 tmpdir/smek.rs)
	sed -i "s/PUT_AESENC_SMEK/${AESENC_SMEK}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_IV_AESENC_SMEK/${AESENC_IV_SMEK}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_RS_AESENC_SMEK/${AESENC_RS_SMEK}/" "${primary_cert_info[config]}" 
	sed "s/PUT_SIZE_AESENC_SMEK/$((${#AESENC_SMEK}/2))/" -i "${primary_cert_info[config]}"

	[[ ${smek_info[flag]} == "yes" ]] && active_flag=${ENABLE_VAL} || active_flag=${DISABLE_VAL} 
	[[ ${smek_info[wp]} == "yes" ]] && wp_flag=${ENABLE_VAL} || wp_flag=${DISABLE_VAL} 
	[[ ${smek_info[rp]} == "yes" ]] && rp_flag=${ENABLE_VAL} || rp_flag=${DISABLE_VAL} 
	[[ ${smek_info[ovrd]} == "yes" ]] && ovrd_flag=${ENABLE_VAL} || ovrd_flag=${DISABLE_VAL} 

	# <wp> <rp> <override> <active/inavtive>
	sed -i "s/PUT_ACTFLAG_AESENC_SMEK/$((16#${wp_flag}${rp_flag}${ovrd_flag}${active_flag}))/" "${primary_cert_info[config]}"

	[[ ${mek_opt_info[flag]} == "yes" ]] && active_flag=${ENABLE_VAL} || active_flag=${DISABLE_VAL} 
	# <wp> <rp> <override> <active/inavtive>
	sed -i "s/PUT_ACTFLAG_PLAIN_MEK_OPT/$((16#${DISABLE_VAL}${DISABLE_VAL}${DISABLE_VAL}${active_flag}))/" "${primary_cert_info[config]}"
	sed -i "s/PUT_PLAIN_MEK_OPT/${mek_opt_info[val]}/" "${primary_cert_info[config]}"

	[[ ${msv_info[flag]} == "yes" ]] && active_flag=${ENABLE_VAL} || active_flag=${DISABLE_VAL} 
	[[ ${msv_info[wp]} == "yes" ]] && wp_flag=${ENABLE_VAL} || wp_flag=${DISABLE_VAL} 
	[[ ${msv_info[rp]} == "yes" ]] && rp_flag=${ENABLE_VAL} || rp_flag=${DISABLE_VAL} 
	[[ ${msv_info[ovrd]} == "yes" ]] && ovrd_flag=${ENABLE_VAL} || ovrd_flag=${DISABLE_VAL} 

	# <wp> <rp> <override> <active/inavtive>
	sed -i "s/PUT_ACTFLAG_PLAIN_MSV/$((16#${wp_flag}${rp_flag}${ovrd_flag}${active_flag}))/" "${primary_cert_info[config]}"
	sed -i "s/PUT_PLAIN_MSV/${msv_info[val]}/" "${primary_cert_info[config]}"

	[[ ${key_rev_info[flag]} == "yes" ]] && active_flag=${ENABLE_VAL} || active_flag=${DISABLE_VAL} 
	[[ ${key_rev_info[wp]} == "yes" ]] && wp_flag=${ENABLE_VAL} || wp_flag=${DISABLE_VAL} 
	[[ ${key_rev_info[rp]} == "yes" ]] && rp_flag=${ENABLE_VAL} || rp_flag=${DISABLE_VAL} 
	[[ ${key_rev_info[ovrd]} == "yes" ]] && ovrd_flag=${ENABLE_VAL} || ovrd_flag=${DISABLE_VAL} 


	# <wp> <rp> <override> <active/inavtive>
	sed -i "s/PUT_ACTFLAG_PLAIN_KEY_REV/$((16#${wp_flag}${rp_flag}${ovrd_flag}${active_flag}))/" "${primary_cert_info[config]}"
	sed -i "s/PUT_PLAIN_KEY_REV/${key_rev_info[val]}/" "${primary_cert_info[config]}"

	[[ ${key_cnt_info[flag]} == "yes" ]] && active_flag=${ENABLE_VAL} || active_flag=${DISABLE_VAL} 
	[[ ${key_cnt_info[wp]} == "yes" ]] && wp_flag=${ENABLE_VAL} || wp_flag=${DISABLE_VAL} 
	[[ ${key_cnt_info[rp]} == "yes" ]] && rp_flag=${ENABLE_VAL} || rp_flag=${DISABLE_VAL} 
	[[ ${key_cnt_info[ovrd]} == "yes" ]] && ovrd_flag=${ENABLE_VAL} || ovrd_flag=${DISABLE_VAL} 

	# <wp> <rp> <override> <active/inavtive>
	sed -i "s/PUT_ACTFLAG_PLAIN_KEY_CNT/$((16#${wp_flag}${rp_flag}${ovrd_flag}${active_flag}))/" "${primary_cert_info[config]}"
	sed -i "s/PUT_PLAIN_KEY_CNT/${key_cnt_info[val]}/" "${primary_cert_info[config]}"

	[[ ${swrev_sysfw_info[flag]} == "yes" ]] && active_flag=${ENABLE_VAL} || active_flag=${DISABLE_VAL} 
	[[ ${swrev_sysfw_info[wp]} == "yes" ]] && wp_flag=${ENABLE_VAL} || wp_flag=${DISABLE_VAL} 
	[[ ${swrev_sysfw_info[rp]} == "yes" ]] && rp_flag=${ENABLE_VAL} || rp_flag=${DISABLE_VAL} 
	[[ ${swrev_sysfw_info[ovrd]} == "yes" ]] && ovrd_flag=${ENABLE_VAL} || ovrd_flag=${DISABLE_VAL} 

	# <wp> <rp> <override> <active/inavtive>
	sed -i "s/PUT_ACTFLAG_PLAIN_SWREV_SYSFW/$((16#${wp_flag}${rp_flag}${ovrd_flag}${active_flag}))/" "${primary_cert_info[config]}"
	sed -i "s/PUT_PLAIN_SWREV_SYSFW/${swrev_sysfw_info[val]}/" "${primary_cert_info[config]}"


	[[ ${swrev_sbl_info[flag]} == "yes" ]] && active_flag=${ENABLE_VAL} || active_flag=${DISABLE_VAL} 
	[[ ${swrev_sbl_info[wp]} == "yes" ]] && wp_flag=${ENABLE_VAL} || wp_flag=${DISABLE_VAL} 
	[[ ${swrev_sbl_info[rp]} == "yes" ]] && rp_flag=${ENABLE_VAL} || rp_flag=${DISABLE_VAL} 
	[[ ${swrev_sbl_info[ovrd]} == "yes" ]] && ovrd_flag=${ENABLE_VAL} || ovrd_flag=${DISABLE_VAL} 

	# <wp> <rp> <override> <active/inavtive>
	sed -i "s/PUT_ACTFLAG_PLAIN_SWREV_SBL/$((16#${wp_flag}${rp_flag}${ovrd_flag}${active_flag}))/" "${primary_cert_info[config]}"
	sed -i "s/PUT_PLAIN_SWREV_SBL/${swrev_sbl_info[val]}/" "${primary_cert_info[config]}"

	[[ ${swrev_sec_brdcfg_info[flag]} == "yes" ]] && active_flag=${ENABLE_VAL} || active_flag=${DISABLE_VAL} 
	[[ ${swrev_sec_brdcfg_info[wp]} == "yes" ]] && wp_flag=${ENABLE_VAL} || wp_flag=${DISABLE_VAL} 
	[[ ${swrev_sec_brdcfg_info[rp]} == "yes" ]] && rp_flag=${ENABLE_VAL} || rp_flag=${DISABLE_VAL} 
	[[ ${swrev_sec_brdcfg_info[ovrd]} == "yes" ]] && ovrd_flag=${ENABLE_VAL} || ovrd_flag=${DISABLE_VAL} 

	# <wp> <rp> <override> <active/inavtive>
	sed -i "s/PUT_ACTFLAG_PLAIN_SWREV_SEC_BRDCFG/$((16#${wp_flag}${rp_flag}${ovrd_flag}${active_flag}))/" "${primary_cert_info[config]}"
	sed -i "s/PUT_PLAIN_SWREV_SEC_BRDCFG/${swrev_sec_brdcfg_info[val]}/" "${primary_cert_info[config]}"

	sed -i "s/PUT_PLAIN_KEYWR_MIN_VER/${KEYWRITER_VERSION}/" "${primary_cert_info[config]}"

	if [[ ${extotp_info[flag]} == "yes" ]]; then
		AESENC_EXTOTP=$(xxd -p -c 1000 tmpdir/aesenc_extotp.enc)
		AESENC_IV_EXTOTP=$(xxd -p -c 1000 tmpdir/extotp.iv)
		AESENC_RS_EXTOTP=$(xxd -p -c 1000 tmpdir/extotp.rs)
		sed -i "s/PUT_AESENC_USER_OTP/${AESENC_EXTOTP}/" "${primary_cert_info[config]}"
		sed -i "s/PUT_IV_AESENC_USER_OTP/${AESENC_IV_EXTOTP}/" "${primary_cert_info[config]}"
		sed -i "s/PUT_RS_AESENC_USER_OTP/${AESENC_RS_EXTOTP}/" "${primary_cert_info[config]}"
		sed -i "s/PUT_WPRP_AESENC_USER_OTP/${extotp_info[wprp]}/" "${primary_cert_info[config]}"
		sed -i "s/PUT_INDX_AESENC_USER_OTP/${extotp_info[index]}/" "${primary_cert_info[config]}"
		sed -i "s/PUT_SIZE_AESENC_USER_OTP/${extotp_info[size]}/" "${primary_cert_info[config]}"
		sed -i "s/PUT_ACTFLAG_AESENC_USER_OTP/$((16#${DISABLE_VAL}${DISABLE_VAL}${DISABLE_VAL}${ENABLE_VAL}))/" "${primary_cert_info[config]}"
	else
		AESENC_EXTOTP=$(string_of_zeros 256)
		AESENC_IV_EXTOTP=$(string_of_zeros 32)
		AESENC_RS_EXTOTP=$(string_of_zeros 64)

		sed -i "s/PUT_AESENC_USER_OTP/${AESENC_EXTOTP}/" "${primary_cert_info[config]}"
		sed -i "s/PUT_IV_AESENC_USER_OTP/${AESENC_IV_EXTOTP}/" "${primary_cert_info[config]}"
		sed -i "s/PUT_RS_AESENC_USER_OTP/${AESENC_RS_EXTOTP}/" "${primary_cert_info[config]}"
		sed -i "s/PUT_WPRP_AESENC_USER_OTP/${extotp_info[wprp]}/" "${primary_cert_info[config]}"
		sed -i "s/PUT_INDX_AESENC_USER_OTP/${extotp_info[index]}/" "${primary_cert_info[config]}"
		sed -i "s/PUT_SIZE_AESENC_USER_OTP/${extotp_info[size]}/" "${primary_cert_info[config]}"
		sed -i "s/PUT_ACTFLAG_AESENC_USER_OTP/$((16#${DISABLE_VAL}${DISABLE_VAL}${DISABLE_VAL}${DISABLE_VAL}))/" "${primary_cert_info[config]}"

	fi
}

# populate_config_secondary
populate_config_secondary(){
	ENC_BMPK_SIGNED_AES_KEY=$(xxd -p -c 1000000 tmpdir/enc_bmpk_signed_aes_key.enc)
	sed -i "s/PUT_ENC_BMPK_SIGNED_AES_KEY/${ENC_BMPK_SIGNED_AES_KEY}/" "${primary_cert_info[config]}" 
	sed "s/PUT_SIZE_ENC_BMPK_SIGNED_AES/$((${#ENC_BMPK_SIGNED_AES_KEY}/2))/" -i "${primary_cert_info[config]}"

	AESENC_BMPKH=$(xxd -p -c 1000000 tmpdir/aesenc_bmpkh.enc)
	AESENC_IV_BMPKH=$(xxd -p -c 1000000 tmpdir/bmpkh.iv)
	AESENC_RS_BMPKH=$(xxd -p -c 1000000 tmpdir/bmpkh.rs)
	sed -i "s/PUT_AESENC_BMPKH/${AESENC_BMPKH}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_IV_AESENC_BMPKH/${AESENC_IV_BMPKH}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_RS_AESENC_BMPKH/${AESENC_RS_BMPKH}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_SIZE_AESENC_BMPKH/$((${#AESENC_BMPKH}/2))/" "${primary_cert_info[config]}"
	
	[[ ${bmpk_info[flag]} == "yes" ]] && active_flag=${ENABLE_VAL} || active_flag=${DISABLE_VAL} 
	[[ ${bmpk_info[wp]} == "yes" ]] && wp_flag=${ENABLE_VAL} || wp_flag=${DISABLE_VAL} 
	[[ ${bmpk_info[rp]} == "yes" ]] && rp_flag=${ENABLE_VAL} || rp_flag=${DISABLE_VAL} 
	[[ ${bmpk_info[ovrd]} == "yes" ]] && ovrd_flag=${ENABLE_VAL} || ovrd_flag=${DISABLE_VAL} 

	# <wp> <rp> <override> <active/inavtive>
	sed -i "s/PUT_ACTFLAG_AESENC_BMPKH/$((16#${wp_flag}${rp_flag}${ovrd_flag}${active_flag}))/" "${primary_cert_info[config]}"

	AESENC_BMEK=$(xxd -p -c 1000000 tmpdir/aesenc_bmek.enc)
	AESENC_IV_BMEK=$(xxd -p -c 1000000 tmpdir/bmek.iv)
	AESENC_RS_BMEK=$(xxd -p -c 1000000 tmpdir/bmek.rs)
	sed -i "s/PUT_AESENC_BMEK/${AESENC_BMEK}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_IV_AESENC_BMEK/${AESENC_IV_BMEK}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_RS_AESENC_BMEK/${AESENC_RS_BMEK}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_SIZE_AESENC_BMEK/$((${#AESENC_BMEK}/2))/" "${primary_cert_info[config]}"
	
	[[ ${bmek_info[flag]} == "yes" ]] && active_flag=${ENABLE_VAL} || active_flag=${DISABLE_VAL} 
	[[ ${bmek_info[wp]} == "yes" ]] && wp_flag=${ENABLE_VAL} || wp_flag=${DISABLE_VAL} 
	[[ ${bmek_info[rp]} == "yes" ]] && rp_flag=${ENABLE_VAL} || rp_flag=${DISABLE_VAL} 
	[[ ${bmek_info[ovrd]} == "yes" ]] && ovrd_flag=${ENABLE_VAL} || ovrd_flag=${DISABLE_VAL} 

	# <wp> <rp> <override> <active/inavtive>
	sed -i "s/PUT_ACTFLAG_AESENC_BMEK/$((16#${wp_flag}${rp_flag}${ovrd_flag}${active_flag}))/" "${primary_cert_info[config]}"

}


