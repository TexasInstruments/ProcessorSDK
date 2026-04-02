ifneq ($(TARGET_PLATFORM),PC)
ifeq ($(TARGET_OS),LINUX)

include $(PRELUDE)
TARGET      := app_utils_network_api
TARGETTYPE  := library

CSOURCES    := network_api.c

IDIRS += $(APP_UTILS_PATH)
IDIRS += $(IMAGING_PATH)/utils/itt_server/include
IDIRS += $(IMAGING_PATH)/utils/network_api/include
IDIRS += $(IMAGING_PATH)/sensor_drv/include

include $(FINALE)

endif
endif
