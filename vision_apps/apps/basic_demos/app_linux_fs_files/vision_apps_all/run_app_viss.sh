echo "!Starting VISS APP Stress Test!"
/opt/vision_apps/vx_app_viss.out 0 5
echo "!10 Second Cooldown!"
sleep 10
/opt/vision_apps/vx_app_viss.out 50 5
echo "!10 Second Cooldown!"
sleep 10
/opt/vision_apps/vx_app_viss.out 100 5
echo "!Finished VISS APP Stress Test!"
