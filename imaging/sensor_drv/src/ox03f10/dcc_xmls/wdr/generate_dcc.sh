DCC_TOOL_PATH=../../../../../tools/dcc_tools/
OUT_PATH=../../../../include

rm -f *.bin
bin_folder=../../dcc_bins/
if [ ! -d "$bin_folder" ]
then
    mkdir "$bin_folder"
fi

bin_folder=../../dcc_bins/wdr/
if [ ! -d "$bin_folder" ]
then
    mkdir "$bin_folder"
fi

rm -f $OUT_PATH/dcc_viss_ox03f10_wdr.h
$DCC_TOOL_PATH/dcc_gen_linux ox03f10_rgb2rgb_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux ox03f10_h3a_aewb_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux ox03f10_viss_nsf4.xml
$DCC_TOOL_PATH/dcc_gen_linux ox03f10_viss_blc.xml
$DCC_TOOL_PATH/dcc_gen_linux ox03f10_cfa_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux ox03f10_cfg_rawfe_wb1.xml
$DCC_TOOL_PATH/dcc_gen_linux ox03f10_wdr_decompand_14b_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux ox03f10_wdr_glbce_dcc_p0.xml
$DCC_TOOL_PATH/dcc_gen_linux ox03f10_h3a_mux_luts_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux ox03f10_viss_gamma.xml
# cp *.bin $bin_folder/
cat *.bin > $bin_folder/dcc_viss_wdr_14b.bin
$DCC_TOOL_PATH/dcc_bin2c $bin_folder/dcc_viss_wdr_14b.bin $OUT_PATH/dcc_viss_ox03f10_wdr.h dcc_viss_ox03f10_wdr
echo; echo
rm -f *.bin

rm -f $OUT_PATH/dcc_2a_ox03f10_wdr.h
$DCC_TOOL_PATH/dcc_gen_linux ox03f10_awb_alg_ti3_tuning.xml
$DCC_TOOL_PATH/dcc_gen_linux ox03f10_h3a_aewb_dcc.xml
# cp *.bin $bin_folder/
cat *.bin > $bin_folder/dcc_2a_wdr.bin
$DCC_TOOL_PATH/dcc_bin2c $bin_folder/dcc_2a_wdr.bin $OUT_PATH/dcc_2a_ox03f10_wdr.h dcc_2a_ox03f10_wdr
echo; echo
rm -f *.bin

rm -f $OUT_PATH/dcc_ldc_ox03f10_wdr.h
$DCC_TOOL_PATH/dcc_gen_linux ox03f10_mesh_ldc_dcc.xml
# cp *.bin $bin_folder/
cat *.bin > $bin_folder/dcc_ldc_wdr.bin
$DCC_TOOL_PATH/dcc_bin2c $bin_folder/dcc_ldc_wdr.bin $OUT_PATH/dcc_ldc_ox03f10_wdr.h dcc_ldc_ox03f10_wdr
echo; echo
rm -f *.bin
