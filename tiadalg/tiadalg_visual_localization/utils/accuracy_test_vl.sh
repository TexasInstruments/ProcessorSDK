TAP_FILE_DIR=/data/adas_vision_data1/users/deepak/adalg_test_report/sdk_0.9/mpV0Skp50PF0/TapFiles
VISION_APPS_DIR=/data/files/ti/vx-work/vision_apps/out/PC/x86_64/LINUX/release/
RELEASE_VER=sdk_0.9

rm -rf ./test_dir_vl/
rm vl_acc_test.tar.gz

mkdir -p test_dir_vl/ext_feat
python gen_feat_bin.py -i $TAP_FILE_DIR -o ./test_dir_vl/ext_feat 2>&1 | tee ./test_dir_vl/ext_feat/feat_log.txt

mkdir -p test_dir_vl/vl_out

$VISION_APPS_DIR/vx_app_visual_localization --cfg ./app_vl_acc.cfg 2>&1 | tee ./test_dir_vl/vl_out/log_sdk_exec.txt

mkdir -p test_dir_vl/result
python compare_results.py -i ./test_dir_vl/vl_out/log.txt -j $TAP_FILE_DIR 2>&1 | tee ./test_dir_vl/result/acc_log.txt

mkdir -p test_dir_vl/scripts
cp compare_results.py ./test_dir_vl/scripts/
cp gen_feat_bin.py ./test_dir_vl/scripts/
cp gen_map_bin.py ./test_dir_vl/scripts/

tar -czf ./vl_acc_test.tar.gz test_dir_vl

cp ./vl_acc_test.tar.gz /data/adas_vision_data1/users/deepak/adalg_test_report/$RELEASE_VER
