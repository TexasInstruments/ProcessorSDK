set TAP_FILE_DIR=D:\data\mpV0Skp1_1-200_PF2\TapFiles
set EXE_NAME=D:\j7_ad_algos\out\tiadalg_visual_localization.exe
set RELEASE_VER=sdk_0.9

rd -r test_dir_vl
del vl_acc_test.tar.gz

mkdir -p test_dir_vl\ext_feat
python gen_feat_bin.py -i %TAP_FILE_DIR% -o ./test_dir_vl/ext_feat 2>&1 | tee ./test_dir_vl/ext_feat/feat_log.txt

mkdir test_dir_vl\vl_out

%EXE_NAME% .\vl_acc_standalone_pf.cfg 2>&1 | tee ./test_dir_vl/vl_out/log_sdk_exec.txt

mkdir -p test_dir_vl\result
python compare_results.py -i ./test_dir_vl/log.txt -j %TAP_FILE_DIR% 2>&1 | tee ./test_dir_vl/result/acc_log.txt

mkdir -p test_dir_vl\scripts
cp compare_results.py .\test_dir_vl\scripts\
cp gen_feat_bin.py .\test_dir_vl\scripts\
cp gen_map_bin.py .\test_dir_vl\scripts\

tar -czf ./vl_acc_test.tar.gz test_dir_vl

cp ./vl_acc_test.tar.gz /data/adas_vision_data1/users/deepak/adalg_test_report/$RELEASE_VER
