#!/bin/bash

echo "#######   ###           ######  ####### #    #          ######  ####### #     # #######  #####"
echo "   #       #            #     #    #    #   #           #     # #       ##   ## #     # #     #"
echo "   #       #            #     #    #    #  #            #     # #       # # # # #     # #"
echo "   #       #            ######     #    ###             #     # #####   #  #  # #     #  #####"
echo "   #       #            #          #    #  #            #     # #       #     # #     #       #"
echo "   #       #            #          #    #   #           #     # #       #     # #     # #     #"
echo "   #      ###           #          #    #    #          ######  ####### #     # #######  #####"

echo
echo Please choose a demo from the following:
echo ===========================================================
echo PTK Demo 1: Surround Radar based Occupancy Grid Map
echo PTK Demo 2: Surround Radar based Dempster-Shafer Occupancy Grid Map
echo PTK Demo 3: Lidar based Occupancy Grid Map
echo PTK Demo 4: Lidar based Dempster-Shafer Occupancy Grid Map
echo PTK Demo 5: SFM based Occupancy Grid Map
echo PTK Demo 6: Multi-sensor fusion based Valet Parking
echo PTK Demo 7: Multi-sensor fusion based Valet Parking with Dempster-Shafer Occupancy Grid Mapping
echo PTK Demo 8: Stereo
echo PTK Demo 9: Stereo Obstacle Detection
echo =====================================================
echo

read -p 'Enter a Demo # in the range [1..9]: ' demo_num

app_dir=/opt/vision_apps/
cfg_dir=/opt/vision_apps/ptk_app_cfg

echo
case $demo_num in
    1)
        echo Running Surround Radar based Occupancy Grid Map demo
        $app_dir/vx_app_surround_radar_ogmap.out --cfg $cfg_dir/app_surround_radar_ogmap/config/app.cfg
    ;;

    2)
        echo Running Surround Radar based Dempster-Shafer Occupancy Grid Map demo
        $app_dir/vx_app_surround_radar_ogmap.out --cfg $cfg_dir/app_surround_radar_ogmap/config/ds_app.cfg
    ;;
    3)
        echo Running Lidar based Occupancy Grid Map demo
        $app_dir/vx_app_lidar_ogmap.out --cfg $cfg_dir/app_lidar_ogmap/config/app.cfg
    ;;
    4)
        echo Running Lidar based Dempster-Shafer Occupancy Grid Map demo
        $app_dir/vx_app_lidar_ogmap.out --cfg $cfg_dir/app_lidar_ogmap/config/ds_app.cfg
    ;;
    5)
        echo SFM based Occupancy Grid Map demo
        $app_dir/vx_app_dof_sfm_fisheye.out --cfg $cfg_dir/app_dof_sfm_fisheye/config/app_evm.cfg
    ;;
    6)
        echo Multi-sensor fusion based Valet Parking demo
        $app_dir/vx_app_valet_parking.out --cfg $cfg_dir/app_valet_parking/config/app_evm.cfg
    ;;
    7)
        echo Multi-sensor fusion based Valet Parking demo with Dempster-Shafer Occupancy Grid Mapping
        $app_dir/vx_app_valet_parking.out --cfg $cfg_dir/app_valet_parking/config/ds_app_evm.cfg
    ;;
    8)
        echo Stereo demo
        $app_dir/vx_app_sde.out --cfg $cfg_dir/app_sde/config/app.cfg
    ;;
    9)
        echo Stereo Obstacle Detection demo
        $app_dir/vx_app_sde_obstacle_detection.out --cfg $cfg_dir/app_sde_obstacle_detection/config/app.cfg
    ;;
    *)
        echo "Please enter a valid demo choice in the range [1..9]"
    ;;
esac
