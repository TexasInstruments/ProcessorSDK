#!/bin/bash

#
# Script to run all basic demos in sequence
# =========================================
#
# Steps to use,
#
# - Make sure input data is stored at path /ti/j7/workarea/tiovx/conformance_tests/test_data/psdkra/
#   - some demo config files hard code this path
# - edit APP_CONFIG_BASE_PATH to point to vision_apps folder
# - edit LD_LIBRARY_PATH to point to glew shared object folder
# - execute this file from vision_apps/tools/scripts (i.e directory where this file is present)
# - output is stored in vision_apps/test_out
#

export TEST_DATA=/ti/j7/workarea/tiovx/conformance_tests/test_data/psdkra/
export APP_CONFIG_BASE_PATH=/ti/j7/workarea/vision_apps
export TIAP_DATABASE_PATH=/ti/j7/test_data/database
export LD_LIBRARY_PATH=/ti/j7/workarea/glew-2.0.0/lib/:/usr/lib64:/usr/lib

make_test_output_folder() {
    mkdir -p $output_folder
}

run_app_tidl() {
    echo "Running app_tidl"
    cp apps/basic_demos/app_tidl/config/app_oc.cfg $executable_folder/app_tidl_oc.cfg
    cp apps/basic_demos/app_tidl/config/app_od.cfg $executable_folder/app_tidl_od.cfg
    cd $executable_folder
    echo "Running app_tidl app_tidl_oc.cfg"
    ./vx_app_tidl --cfg app_tidl_oc.cfg > out_tidl_oc.txt
    cd -
    cp $executable_folder/out_tidl*.txt $output_folder/
    echo "Running app_tidl ... DONE!!!"
}

run_app_tidl_object_detect() {
    echo "Running run_app_tidl_object_detect"
    echo "Running parking spot detect"
    cp apps/dl_demos/app_tidl_object_detection/config/app.cfg $executable_folder/app_tidl_psd.cfg
    cd $executable_folder
    mkdir -p psd_out/
    ./vx_app_tidl_object_detection --cfg app_tidl_psd.cfg
    cd -
    cp -r $executable_folder/psd_out $output_folder/
    echo "Running vehicle detect"
    cp apps/dl_demos/app_tidl_object_detection/config/app_vd.cfg $executable_folder/app_tidl_vd.cfg
    cd $executable_folder
    mkdir -p vd_out/
    ./vx_app_tidl_object_detection --cfg app_tidl_vd.cfg
    cd -
    cp -r $executable_folder/vd_out $output_folder/
    echo "Running run_app_tidl_object_detect ... DONE!!!"
}

run_app_c7x() {
    echo "Running app_c7x_kernel"
    cp apps/basic_demos/app_c7x_kernel/config/app.cfg $executable_folder/app_c7x.cfg
    cp $TEST_DATA/app_c7x/*.* $executable_folder/
    cd $executable_folder
    ./vx_app_c7x_kernel --cfg app_c7x.cfg
    cd -
    cp $executable_folder/app_c7x_out_img.* $output_folder/
    echo "Running app_c7x_kernel ... DONE!!!"
}

run_app_ldc() {
    echo "Running app_ldc"
    cp apps/basic_demos/app_ldc/config/app.cfg $executable_folder/app_ldc.cfg
    cp apps/basic_demos/app_ldc/config/mesh.txt $executable_folder/
    cd $executable_folder
    ./vx_app_ldc --cfg app_ldc.cfg
    cd -
    cp -r $executable_folder/ldc_out $output_folder/
    echo "Running app_ldc ... DONE!!!"
}

run_app_dof() {
    echo "Running app_dof"
    cp apps/basic_demos/app_dof/config/app.cfg $executable_folder/app_dof.cfg
    cd $executable_folder
    ./vx_app_dense_optical_flow --cfg app_dof.cfg
    cd -
    cp -r $executable_folder/dof_out $output_folder/
    echo "Running app_dof ... DONE!!!"
}

run_app_ldc_dof() {
    echo "Running app_ldc_dof"
    cd $executable_folder
    ./vx_app_ldc_dof --cfg $APP_CONFIG_BASE_PATH/apps/basic_demos/app_ldc_dof/config/app.cfg
    cd -
    echo "Running app_ldc_dof ... DONE!!!"
}

run_app_stereo() {
    echo "Running app_stereo"
    cp apps/basic_demos/app_stereo/config/app.cfg $executable_folder/app_stereo.cfg
    cd $executable_folder
    mkdir sde_out/
    ./vx_app_stereo_depth --cfg app_stereo.cfg
    cd -
    cp -r $executable_folder/sde_out $output_folder/
    echo "Running app_stereo ... DONE!!!"
}

run_app_sfm_fisheye() {
    echo "Running app_sfm_fisheye"
    cd $executable_folder
    ./vx_app_sfm_fisheye --cfg $APP_CONFIG_BASE_PATH/apps/ptk_demos/app_sfm_fisheye/config/app.cfg
    cd -
    echo "Running app_sfm_fisheye ... DONE!!!"
}

run_app_lidar_ogmap() {
    echo "Running app_lidar_ogmap"
    cd $executable_folder
    ./vx_app_lidar_ogmap --cfg $APP_CONFIG_BASE_PATH/apps/ptk_demos/app_lidar_ogmap/config/app.cfg
    cd -
    echo "Running app_lidar_ogmap ... DONE!!!"
}

run_app_surround_radar_ogmap() {
    echo "Running app_surround_radar_ogmap"
    cd $executable_folder
    ./vx_app_surround_radar_ogmap --cfg $APP_CONFIG_BASE_PATH/apps/ptk_demos/app_surround_radar_ogmap/config/app.cfg
    cd -
    echo "Running app_surround_radar_ogmap ... DONE!!!"
}


run_app_fused_ogmap() {
    echo "Running app_fused_ogmap"
    cd $executable_folder
    ./vx_app_fused_ogmap --cfg $APP_CONFIG_BASE_PATH/apps/ptk_demos/app_fused_ogmap/config/app.cfg
    cd -
    echo "Running app_fused_ogmap ... DONE!!!"
}

run_app_valet_parking() {
    echo "Running app_valet_parking"
    cd $executable_folder
    ./vx_app_valet_parking --cfg $APP_CONFIG_BASE_PATH/apps/ptk_demos/app_valet_parking/config/app.cfg
    cd -
    echo "Running app_valet_parking ... DONE!!!"
}

run_app_radar_gtrack() {
    echo "Running app_radar_gtrack"
    cd $executable_folder
    ./vx_app_radar_gtrack --cfg $APP_CONFIG_BASE_PATH/apps/ptk_demos/app_radar_gtrack/config/app.cfg
    cd -
    echo "Running app_radar_gtrack ... DONE!!!"
}

run_app_tidl_pixel_classification() {
    echo "Running app_tidl_pixel_classification"
    cp apps/dl_demos/app_tidl_pixel_classification/config/app_ds_ss_ms.cfg $executable_folder/
    cd $executable_folder
    mkdir -p ds_ss_ms_out
    ./vx_app_tidl_pixel_classification --cfg app_ds_ss_ms.cfg
    cd -
    cp -r $executable_folder/ds_ss_ms_out $output_folder/
    echo "Running app_tidl_pixel_classification ... DONE!!!"
}

run_app_visual_localization() {
    echo "Running app_visual_localization"
    cp apps/dl_demos/app_visual_localization/config/app_vl_ext_feat.cfg $executable_folder/
    cd $executable_folder
    mkdir -p vl_out
    ./vx_app_visual_localization --cfg app_vl_ext_feat.cfg
    cd -
    cp -r $executable_folder/vl_out $output_folder/
    echo "Running app_visual_localization ... DONE!!!"
}

run_app_single_cam_demo() {
    echo "Running app_single_cam_demo"
    mkdir -p apps/basic_demos/app_single_cam/output
    $executable_folder/vx_app_single_cam --cfg apps/basic_demos/app_single_cam/config/app_IMX390_001.cfg
    $executable_folder/vx_app_single_cam --cfg apps/basic_demos/app_single_cam/config/app_IMX390_002.cfg
    $executable_folder/vx_app_single_cam --cfg apps/basic_demos/app_single_cam/config/app_IMX390_003.cfg
    $executable_folder/vx_app_single_cam --cfg apps/basic_demos/app_single_cam/config/app_ov10640_001.cfg
    cp -r -p apps/basic_demos/app_single_cam/output $output_folder/single_cam_out
    # remove tmp files and folders
    rm -rf apps/basic_demos/app_single_cam/output
    rm vx_app_viss_*.jpg
    rm vx_app_viss_*.txt
    echo "Running app_single_cam ... DONE. !!!"
}

run_app_multi_cam_demo() {
    echo "Running app_multi_cam_demo"
    mkdir -p apps/basic_demos/app_multi_cam/output
    $executable_folder/vx_app_multi_cam --cfg apps/basic_demos/app_multi_cam/app_IMX390_MultiCam.cfg
    cp -r -p apps/basic_demos/app_multi_cam/output $output_folder/multi_cam_out
    echo "Running app_multi_cam ... DONE. !!!"
}

run_all_demos() {
    profile=$1
    executable_folder=./out/PC/x86_64/LINUX/$profile
    output_folder=./test_out/$profile
    make_test_output_folder

# Basic Demos
   run_app_c7x
   run_app_dof
   run_app_ldc
   run_app_tidl
   run_app_stereo
   run_app_single_cam_demo
#   run_app_multi_cam_demo

# TIDL valet parking demos
   run_app_tidl_object_detect
   run_app_tidl_pixel_classification
   run_app_visual_localization

# PTK valet parking demos
    run_app_ldc_dof
    run_app_sfm_fisheye
    run_app_lidar_ogmap
    run_app_surround_radar_ogmap
#    run_app_fused_ogmap
    run_app_valet_parking    
    run_app_radar_gtrack
}

run_single_demo() {
    profile=$1
    executable_folder=./out/PC/x86_64/LINUX/$profile
    output_folder=./test_out/$profile
    make_test_output_folder
}

cd ../..
run_all_demos debug
#run_all_demos release
#run_single_demo debug
cd -
