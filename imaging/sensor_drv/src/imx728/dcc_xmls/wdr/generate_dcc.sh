DCC_TOOL_PATH=../../../../../tools/dcc_tools/
DCC_BIN_PATH=../../dcc_bins
OUT_PATH=../../../../include

mkdir -p ${DCC_BIN_PATH}
rm -f ${DCC_BIN_PATH}/*.bin
rm -f *.bin
rm -f $OUT_PATH/dcc_viss_imx728_wdr.h
# $DCC_TOOL_PATH/dcc_gen_linux IMX728_ipipe_rgb2rgb_1_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX728_rgb2rgb_dcc.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX728_viss_h3a_aewb_cfg.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX728_viss_nsf4.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX728_viss_ee.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX728_viss_blc.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX728_flxd_cfa.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX728_rawfe_decompand.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX728_viss_h3a_mux_luts_cfg.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX728_viss_glbce.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX728_viss_gamma.xml
cat *.bin > ${DCC_BIN_PATH}/dcc_viss_wdr.bin
$DCC_TOOL_PATH/dcc_bin2c ${DCC_BIN_PATH}/dcc_viss_wdr.bin $OUT_PATH/dcc_viss_imx728_wdr.h dcc_viss_imx728_wdr

rm -f *.bin
rm -f $OUT_PATH/dcc_2a_imx728_wdr.h
$DCC_TOOL_PATH/dcc_gen_linux IMX728_awb_alg_ti3_tuning.xml
$DCC_TOOL_PATH/dcc_gen_linux IMX728_viss_h3a_aewb_cfg.xml
cat *.bin > ${DCC_BIN_PATH}/dcc_2a_wdr.bin
$DCC_TOOL_PATH/dcc_bin2c ${DCC_BIN_PATH}/dcc_2a_wdr.bin $OUT_PATH/dcc_2a_imx728_wdr.h dcc_2a_imx728_wdr

rm -f *.bin
rm -f $OUT_PATH/dcc_ldc_imx728_wdr.h
$DCC_TOOL_PATH/dcc_gen_linux IMX728_mesh_ldc_dcc.xml
cat *.bin > ${DCC_BIN_PATH}/dcc_ldc_wdr.bin
$DCC_TOOL_PATH/dcc_bin2c ${DCC_BIN_PATH}/dcc_ldc_wdr.bin $OUT_PATH/dcc_ldc_imx728_wdr.h dcc_ldc_imx728_wdr

rm -f *.bin
