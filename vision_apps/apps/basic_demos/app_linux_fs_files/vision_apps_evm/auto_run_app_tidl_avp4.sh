cd /ti_fs/vision_apps 

PROCESSOR=`uname -a`
if [[ ${PROCESSOR} == *_"J721S2"_* ]]; then
    # Disable unused devices for ECU_BUILD
    k3conf disable device 179 # CODEC0
    k3conf disable device 360 # USB0
    k3conf disable device 28  # CPSW1

    k3conf disable device 170 # PBIST3
    k3conf disable device 171 # PBIST0
    k3conf disable device 172 # PBIST1
    k3conf disable device 173 # PBIST4
    k3conf disable device 178 # MCU_PBIST2
    k3conf disable device 177 # MCU_PBIST1
    k3conf disable device 176 # MCU_PBIST0

    k3conf disable device 2   # ATL0
    k3conf disable device 117 # GPMC0
    k3conf disable device 95  # ELM0
    k3conf disable device 98  # MMCSD0
    k3conf disable device 276 # PCIE1
    k3conf disable device 154 # DSS_DSI0
    k3conf disable device 214 # I2C0
    k3conf disable device 224 # NAVSS0
fi

. ./vision_apps_init.sh 
sleep 8 
( sleep 3; echo "a14" ) | ./run_app_tidl_avp4.sh

