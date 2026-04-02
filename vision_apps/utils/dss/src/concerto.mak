ifeq ($(BUILD_MCU_BOARD_DEPENDENCIES),yes)
ifeq ($(TARGET_OS),$(filter $(TARGET_OS),SYSBIOS FREERTOS SAFERTOS))
ifeq ($(TARGET_CPU),R5F)

include $(PRELUDE)
TARGET      := app_utils_dss
TARGETTYPE  := library

ifeq ($(BUILD_ENABLE_ETHFW),yes)
DEFS+=ENABLE_ETHFW
endif

ifeq ($(RTOS_SDK), mcu_plus_sdk)

IDIRS       += $(MCU_PLUS_SDK_PATH)/source
IDIRS       += $(MCU_PLUS_SDK_PATH)/source/drivers
IDIRS       += $(MCU_PLUS_SDK_PATH)/source/kernel/dpl
IDIRS       += $(VISION_APPS_PATH)/platform/j722s/rtos/mcu2_0/generated/

CSOURCES    := app_dss_mcu_plus.c
else
IDIRS       += $(PDK_PATH)/packages
IDIRS       += $(PDK_PATH)/packages/ti/drv
IDIRS       += $(PDK_PATH)/packages/ti/drv/dss

CSOURCES    := app_dss.c app_dss_soc.c app_dctrl.c app_dss_defaults.c app_dss_dual_display_defaults.c

endif

DEFS+=$(BUILD_PDK_BOARD)

include $(FINALE)

endif
endif
endif
