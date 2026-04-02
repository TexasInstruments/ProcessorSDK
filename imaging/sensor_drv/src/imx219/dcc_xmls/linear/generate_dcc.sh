DCC_TOOL_PATH=../../../../../tools/dcc_tools/
OUT_PATH=../../../../include

if [ -z $1 ]; then
    #ISIZE=1920x1080
    #ISIZE=1640x1232
    #ISIZE=1280x720
    #ISIZE=640x480
    ISIZE=320x240
else
    ISIZE=$1
fi

rm -f *.bin
#rm -f $OUT_PATH/dcc_viss_imx219.h
$DCC_TOOL_PATH/dcc_gen_linux IMX219_rgb2rgb_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX219_viss_otf_dpc.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX219_viss_h3a_aewb_cfg_${ISIZE}.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX219_viss_nsf4.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX219_viss_blc.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX219_flxd_cfa.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX219_rawfe_decompand.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX219_viss_h3a_mux_luts_cfg.xml
cat *.bin > ../../dcc_bins/dcc_viss_${ISIZE}.bin
cp *.bin ../../dcc_bins/linear/
#$DCC_TOOL_PATH/dcc_bin2c ../../dcc_bins/dcc_viss_${ISIZE}.bin $OUT_PATH/dcc_viss_imx219.h dcc_viss_imx219

echo ''
rm -f *.bin
#rm -f $OUT_PATH/dcc_2a_imx219.h
$DCC_TOOL_PATH/dcc_gen_linux IMX219_awb_alg_ti3_tuning.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX219_viss_h3a_aewb_cfg_${ISIZE}.xml
cat *.bin > ../../dcc_bins/dcc_2a_${ISIZE}.bin
cp *.bin ../../dcc_bins/linear/
#$DCC_TOOL_PATH/dcc_bin2c ../../dcc_bins/dcc_2a_${ISIZE}.bin $OUT_PATH/dcc_2a_imx219.h dcc_2a_imx219


echo ''
rm -f *.bin

