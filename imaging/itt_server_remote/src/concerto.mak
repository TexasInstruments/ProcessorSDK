ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), A72 A53 R5F))

include $(PRELUDE)
TARGET      := ti_imaging_ittsrvr
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)

IDIRS       += $(HOST_ROOT)/sensor_drv/include
IDIRS       += $(APP_UTILS_PATH)/utils/remote_service/include
IDIRS       += $(APP_UTILS_PATH)/utils/ipc/include
IDIRS       += $(HOST_ROOT)/itt_server_remote/include


include $(FINALE)

endif
