DCC_TOOL_PATH=../../../../../tools/dcc_tools/
OUT_PATH=../../../../include

rm -f *.bin
bin_folder=../../dcc_bins/
if [ ! -d "$bin_folder" ]
then
    mkdir "$bin_folder"
fi

bin_folder=../../dcc_bins/X5C/
if [ ! -d "$bin_folder" ]
then
    mkdir "$bin_folder"
fi

bin_folder=../../dcc_bins/X5C/linear/
if [ ! -d "$bin_folder" ]
then
    mkdir "$bin_folder"
fi

rm -f $OUT_PATH/dcc_viss_X5C.h
$DCC_TOOL_PATH/dcc_gen_linux X5C_rgb2rgb_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux X5C_h3a_aewb_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux X5C_viss_nsf4.xml
$DCC_TOOL_PATH/dcc_gen_linux X5C_viss_blc.xml
$DCC_TOOL_PATH/dcc_gen_linux X5C_cfa_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux X5C_viss_gamma_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux X5C_rawfe_lsc_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux X5C_linear_decompand_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux X5C_h3a_mux_luts_dcc.xml
cp *.bin $bin_folder/
cat *.bin > ../../dcc_bins/dcc_viss.bin
$DCC_TOOL_PATH/dcc_bin2c ../../dcc_bins/dcc_viss.bin $OUT_PATH/dcc_viss_X5C.h dcc_viss_X5C
echo; echo
rm -f *.bin
rm -f $OUT_PATH/dcc_2a_X5C.h
$DCC_TOOL_PATH/dcc_gen_linux X5C_awb_alg_ti3_tuning.xml
$DCC_TOOL_PATH/dcc_gen_linux X5C_h3a_aewb_dcc.xml
cp *.bin $bin_folder/
cat *.bin > ../../dcc_bins/dcc_2a.bin
$DCC_TOOL_PATH/dcc_bin2c ../../dcc_bins/dcc_2a.bin $OUT_PATH/dcc_2a_X5C.h dcc_2a_X5C
echo; echo
rm -f *.bin
rm -f $OUT_PATH/dcc_ldc_X5C.h
$DCC_TOOL_PATH/dcc_gen_linux X5C_mesh_ldc_dcc.xml
cp *.bin $bin_folder/
cat *.bin > ../../dcc_bins/dcc_ldc.bin
$DCC_TOOL_PATH/dcc_bin2c ../../dcc_bins/dcc_ldc.bin $OUT_PATH/dcc_ldc_X5C.h dcc_ldc_X5C
echo; echo
rm -f *.bin
