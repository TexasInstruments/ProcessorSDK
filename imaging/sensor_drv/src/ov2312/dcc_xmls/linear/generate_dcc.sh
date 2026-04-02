DCC_TOOL_PATH=../../../../../tools/dcc_tools/
OUT_PATH=../../../../include

rm *.bin
bin_folder=../../dcc_bins/
if [ ! -d "$bin_folder" ]
then
    mkdir "$bin_folder"
fi

bin_folder=../../dcc_bins/OV2312-UB953_LI/
if [ ! -d "$bin_folder" ]
then
    mkdir "$bin_folder"
fi

bin_folder=../../dcc_bins/OV2312-UB953_LI/linear/
if [ ! -d "$bin_folder" ]
then
    mkdir "$bin_folder"
fi

#################################################################
rm $OUT_PATH/dcc_viss_ov2312.h
$DCC_TOOL_PATH/dcc_gen_linux ov2312_rawfe_decompand.xml
$DCC_TOOL_PATH/dcc_gen_linux ov2312_rgb2rgb_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux ov2312_h3a_aewb_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux ov2312_viss_nsf4.xml
$DCC_TOOL_PATH/dcc_gen_linux ov2312_viss_blc.xml
$DCC_TOOL_PATH/dcc_gen_linux ov2312_cfa_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux ov2312_h3a_mux_luts_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux ov2312_wdr_glbce_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux ov2312_viss_ee_dcc.xml

$DCC_TOOL_PATH/dcc_gen_linux rawfe_cfg_dpc_extAM62.xml
$DCC_TOOL_PATH/dcc_gen_linux rawfe_cfg_lsc_extAM62.xml
$DCC_TOOL_PATH/dcc_gen_linux rawfe_cfg_pcid.xml

echo ' '
cp *.bin $bin_folder/
cat *.bin > ../../dcc_bins/dcc_viss.bin
$DCC_TOOL_PATH/dcc_bin2c ../../dcc_bins/dcc_viss.bin $OUT_PATH/dcc_viss_ov2312.h dcc_viss_ov2312
echo ' '


#################################################################
rm *.bin
rm $OUT_PATH/dcc_2a_ov2312_linear.h
$DCC_TOOL_PATH/dcc_gen_linux ov2312_awb_alg_ti3_tuning.xml
$DCC_TOOL_PATH/dcc_gen_linux ov2312_h3a_aewb_dcc.xml

echo ' '
cp *.bin $bin_folder/
cat *.bin > ../../dcc_bins/dcc_2a.bin
$DCC_TOOL_PATH/dcc_bin2c ../../dcc_bins/dcc_2a.bin $OUT_PATH/dcc_2a_ov2312.h dcc_2a_ov2312
echo ' '



#################################################################
#rm *.bin
#rm $OUT_PATH/dcc_ldc_ov2312_linear.h
#$DCC_TOOL_PATH/dcc_gen_linux ov2312_mesh_ldc_dcc.xml
#
#echo ' '
#cp *.bin $bin_folder/
#cat *.bin > ../../dcc_bins/dcc_ldc.bin
#$DCC_TOOL_PATH/dcc_bin2c ../../dcc_bins/dcc_ldc.bin $OUT_PATH/dcc_ldc_ov2312.h dcc_ldc_ov2312
#echo ' '

rm *.bin
echo ''

