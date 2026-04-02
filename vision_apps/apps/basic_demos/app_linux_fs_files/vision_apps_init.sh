export VX_TEST_DATA_PATH=/opt/vision_apps/test_data

# Location of DLR library
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib:/usr/lib/python3.8/site-packages/dlr

# Location of the input data for PTK demos
export TIAP_DATABASE_PATH=/opt/vision_apps/test_data_ptk
# Location of the input data for Stereo demo
export APP_STEREO_DATA_PATH=$VX_TEST_DATA_PATH/psdkra/stereo_test_data

# APP config path for PTK demos
export APP_CONFIG_BASE_PATH=/opt/vision_apps/ptk_app_cfg

# Ensure output directory is available for certain apps
mkdir -p ${VX_TEST_DATA_PATH}/output

/opt/vision_apps/vx_app_arm_remote_log.out &
