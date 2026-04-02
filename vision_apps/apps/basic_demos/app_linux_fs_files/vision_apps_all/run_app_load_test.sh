echo "!Starting App Load Test!"

/opt/vision_apps/vx_app_load_test.out 2 0 5
echo "!10 Second Cooldown!"
sleep 10
/opt/vision_apps/vx_app_load_test.out 2 50 5
echo "!10 Second Cooldown!"
sleep 10
/opt/vision_apps/vx_app_load_test.out 2 100 5
echo "!10 Second Cooldown!"
sleep 10

/opt/vision_apps/vx_app_load_test.out 4 0 5
echo "!10 Second Cooldown!"
sleep 10
/opt/vision_apps/vx_app_load_test.out 4 50 5
echo "!10 Second Cooldown!"
sleep 10
/opt/vision_apps/vx_app_load_test.out 4 100 5
echo "!10 Second Cooldown!"
sleep 10

/opt/vision_apps/vx_app_load_test.out 5 0 5
echo "!10 Second Cooldown!"
sleep 10
/opt/vision_apps/vx_app_load_test.out 5 50 5
echo "!10 Second Cooldown!"
sleep 10
/opt/vision_apps/vx_app_load_test.out 5 100 5

echo "!Finished Load Test!"
