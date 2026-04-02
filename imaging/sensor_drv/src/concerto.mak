ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), R5F))

include $(PRELUDE)
TARGET      := ti_imaging_sensordrv
TARGETTYPE  := library
CSOURCES    += iss_sensors.c
CSOURCES    += imx390/iss_sensor_imx390.c
CSOURCES    += imx390_discovery/iss_sensor_imx390_discovery_narrow.c
CSOURCES    += imx390_discovery/iss_sensor_imx390_discovery_mid.c
CSOURCES    += imx390_discovery/iss_sensor_imx390_discovery_wide.c
CSOURCES    += imx390_discovery/iss_sensor_imx390_discovery_common.c
CSOURCES    += imx390_discovery/d3_eeprom.c
CSOURCES    += imx728/iss_sensor_imx728.c
CSOURCES    += ox03f10/iss_sensor_ox03f10.c
CSOURCES    += ox05c1s/iss_sensor_ox05c1s.c
CSOURCES    += gw_ar0233_yuv/iss_sensor_gw_ar0233.c
CSOURCES    += ub9xx_raw_test_pattern/iss_sensor_raw_testpat.c
CSOURCES    += ub9xx_yuv_test_pattern/iss_sensor_testpat.c
ifeq ($(SOC), $(filter $(SOC), am62a))
CSOURCES    += ov2312/iss_sensor_ov2312.c
else
CSOURCES    += ar0233/iss_sensor_ar0233.c
CSOURCES    += ar0820/iss_sensor_ar0820.c
endif

IDIRS       += $(HOST_ROOT)/sensor_drv/include
IDIRS       += $(APP_UTILS_PATH)/utils/remote_service/include
IDIRS       += $(APP_UTILS_PATH)/utils/ipc/include

ifeq ($(RTOS_SDK), mcu_plus_sdk)
IDIRS       += $(MCU_PLUS_SDK_PATH)/source
else
IDIRS       += $(PDK_PATH)/packages
endif

ifeq ($(SOC), $(filter $(SOC), am62a))
ifeq ($(TISDK_IMAGE), edgeai)
SKIPBUILD=1
endif
endif

include $(FINALE)

endif
