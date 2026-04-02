ifneq ($(TARGET_PLATFORM),PC)
ifeq ($(TARGET_OS),LINUX)

include $(PRELUDE)

TARGET      := app_utils_itt_server
TARGETTYPE  := library

CSOURCES    := itt_server_main.c itt_ctrl_handle_echo.c itt_ctrl_handle_2a.c itt_ctrl_handle_image_save.c itt_ctrl_handle_dcc_send.c itt_ctrl_handle_sensor.c itt_ctrl_handle_dev_ctrl.c itt_ctrl_edge_ai.c

IDIRS += $(IMAGING_PATH)/kernels/include/
IDIRS += $(IMAGING_PATH)/itt_server_remote/include/
IDIRS += $(APP_UTILS_PATH)
IDIRS += $(IMAGING_PATH)/utils/itt_server/include
IDIRS += $(IMAGING_PATH)/utils/network_api/include
IDIRS += $(IMAGING_PATH)/utils/iss/include

include $(FINALE)

endif # TARGET_OS LINUX
endif # TARGET_PLATFORM PC
