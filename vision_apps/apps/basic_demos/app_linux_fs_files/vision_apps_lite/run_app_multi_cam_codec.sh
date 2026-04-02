if [[ -f /opt/vision_apps/test_data/psdkra/app_multi_cam_codec/test_video_1080p30.264 ]]
then
    cp /opt/vision_apps/test_data/psdkra/app_multi_cam_codec/test_video_1080p30.264 /tmp/test_data.264
else
    echo "ERROR=> Test Data Not Found"
    exit -1
fi

/opt/vision_apps/vx_app_multi_cam_codec.out --cfg /opt/vision_apps/app_multi_cam_codec.cfg

for i in 0 1 2 3 4 5 6 7 8 9 10 11
do
    if [[ -s /tmp/output_video_$i.264 ]]
    then
        mv /tmp/output_video_$i.264 /ti_fs/vision_apps/
    fi
done

if [[ -f /tmp/test_data.264 ]]
then
    rm /tmp/test_data.264
fi
