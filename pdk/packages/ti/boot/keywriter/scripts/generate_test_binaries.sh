#!/bin/bash

if [ -z "$PDK_INSTALL_PATH" ]; then
    echo "Error: PDK_INSTALL_PATH is not defined"
    exit
fi

if [ -z "$SOC" ]; then
    echo "Error: SOC is not defined"
    exit
fi

if [ -z "$PDK_KEYWR_COMP_PATH" ]; then
    PDK_KEYWR_COMP_PATH="$PDK_INSTALL_PATH/ti/boot/keywriter"
    echo "PDK_KEYWR_COMP_PATH = $PDK_KEYWR_COMP_PATH"
fi

OUTPUT="$PDK_KEYWR_COMP_PATH/binary/${SOC}/test_images"

mkdir -p $OUTPUT

if [ ! -d $PDK_KEYWR_COMP_PATH/scripts/keys ]; then
	cd $PDK_KEYWR_COMP_PATH/scripts
	./gen_keywr_cert.sh -g
fi

echo "Copy known dummy keys to be used for the test cases"
cp $PDK_INSTALL_PATH/ti/build/makerules/k3_dev_mpk.pem $PDK_INSTALL_PATH/ti/boot/keywriter/scripts/keys/smpk.pem
xxd -p -r $PDK_INSTALL_PATH/ti/build/makerules/k3_dev_mek.txt > $PDK_INSTALL_PATH/ti/boot/keywriter/scripts/keys/smek.key

if [ ! -f $PDK_KEYWR_COMP_PATH/scripts/ti_fek_public.pem ]; then
	echo "Error: ti_fek_public.pem doesn't exist"
	exit
fi

declare -A test1 test2
test1[comment]="Test 1 - multi-stage conversion"
# Generate MSV (Expected PASS)
test1[1]="./gen_keywr_cert.sh --msv 0xC0FFE            -t ti_fek_public.pem"
# Attempt to update MSV without override set (Expected FAIL)
test1[2]=${test1[1]}
# Attempt to override MSV with similar value (Expected PASS)
test1[3]="./gen_keywr_cert.sh --msv 0xC0FFE --msv-ovrd -t ti_fek_public.pem"
# Write-protect MSV (Expected PASS)
test1[4]="./gen_keywr_cert.sh --msv 0xC0FFE --msv-ovrd --msv-wp -t ti_fek_public.pem"
# Attempt to override write-protected MSV (Expected FAIL)
test1[5]="./gen_keywr_cert.sh --msv 0xC0FFE --msv-ovrd -t ti_fek_public.pem"
# Override SWREV values. Keep SYSFW SWREV at 1, bump SBL SWREV (Expected PASS)
# Note: Requires SWREV update in tiboot3.bin certificate
test1[6]="./gen_keywr_cert.sh -t ti_fek_public.pem --sr-sbl 3 --sr-sysfw 1 --sr-sbl-ovrd --sr-sysfw-ovrd"
# Override Security boardcfg SWREV (Expected PASS)
# Note: Requires SWREV update in Security boardcfg certificate/extended boot data
test1[7]="./gen_keywr_cert.sh -t ti_fek_public.pem --sr-bcfg 5 --sr-bcfg-ovrd"
# Generate pattern for blowing into beginning of Extended OTP (Expected PASS)
test1[8]="./construct_ext_otp_data.sh -extotp 0x80000001 -indx 0 -size 32;
          ./gen_keywr_cert.sh -t ti_fek_public.pem --ext-otp ext_otp_data.bin --ext-otp-indx 0 --ext-otp-size 32 --ext-otp-wprp 0x1"
test1[9]=${test1[8]}
# Generate pattern for blowing into offset from Extended OTP base (Expected PASS)
test1[10]="./construct_ext_otp_data.sh -extotp 0x81 -indx 32 -size 8;
          ./gen_keywr_cert.sh -t ti_fek_public.pem --ext-otp ext_otp_data.bin --ext-otp-indx 32 --ext-otp-size 8 "
# Blow backup keyset with write-protection (Expected PASS) and attempt to blow again (Expected FAIL)
test1[11]="./gen_keywr_cert.sh -t ti_fek_public.pem -b keys/bmpk.pem --bmek keys/bmek.key -b-wp --bmek-wp --mek-opt 0x1 --mpk-opt 0x21"
test1[12]=${test1[11]}
# Blow secondary keyset with write-protection (Expected PASS) and attempt to blow again (Expected FAIL)
test1[13]="./gen_keywr_cert.sh -t ti_fek_public.pem -s keys/smpk.pem --smek keys/smek.key -s-wp --smek-wp --mek-opt 0x1 --mpk-opt 0x21"
test1[14]=${test1[13]}
# Set Key count to 2 (Expected PASS)
test1[15]="./gen_keywr_cert.sh -t ti_fek_public.pem --keycnt 2 --keycnt-wp "
# Set Key revision to 1 (Expected PASS)
# Note: Device is now HS-SE
test1[16]="./gen_keywr_cert.sh -t ti_fek_public.pem --keyrev 1 "

test2[comment]="Test 2 - single-shot converstion"
# Blow secondary key set, backup key set key count, and key revision in one go
# Note: Device is now HS-SE
test2[1]="./gen_keywr_cert.sh -t ti_fek_public.pem
            -s keys/smpk.pem --smek keys/smek.key -s-rp --smek-rp
            -b keys/bmpk.pem --bmek keys/bmek.key -b-rp --bmek-rp
            --mek-opt 0x1 --mpk-opt 0x21
            --keycnt 2 --keycnt-wp
            --keyrev 1" 

tests=(test1 test2)

echo "make keywriter_img (initial)"
make -C $PDK_INSTALL_PATH/ti/build keywriter_img

function gen_tests()
{
    local -n test_array=$1
    echo "Generating test seqeuence $2"

    # Create a unique test directory for storing the test binaries
    TEST_OUT_DIR=$OUTPUT/test$2
    mkdir -p $TEST_OUT_DIR

    # Dump the test description to into this directory
    echo "${test_array[comment]}" > $TEST_OUT_DIR/test_description.txt

    for i in $(seq 1 ${#test_array[@]})
    do
        i2=$(printf "%02d" $i)
        cd $PDK_KEYWR_COMP_PATH/scripts
        case "$i" in
            "${#test_array[@]}")
                # Don't generate the last iteration since there is a comment key
                continue
                ;;
            *)
                echo "${test_array[$i]}" |& tee -a $TEST_OUT_DIR/test_description.txt
                eval ${test_array[$i]} &> $TEST_OUT_DIR/kw_test${i2}.log
                ;;
        esac

        echo "# Generating test$2 Binary No. ${i}"
        cp $PDK_KEYWR_COMP_PATH/x509cert/final_certificate.bin $TEST_OUT_DIR/cert-kw_test${i2}.bin
        cd $PDK_INSTALL_PATH/ti/build
        echo " make keywriter_img with TC ${i2}"
        make keywriter_img &>> $TEST_OUT_DIR/kw_test${i2}.log
        cp $PDK_KEYWR_COMP_PATH/binary/${SOC}/keywriter_img_${SOC}_release.tiimage $TEST_OUT_DIR/kw_test${i2}.tiimage
        cp $PDK_KEYWR_COMP_PATH/binary/${SOC}/keywriter_img_${SOC}_release.xer5f  $TEST_OUT_DIR/kw_test${i2}.xer5f

        echo "# Done!"
    done
}

for i in $(seq 1 ${#tests[@]}); do
    gen_tests ${tests[$((i-1))]} $i
done
