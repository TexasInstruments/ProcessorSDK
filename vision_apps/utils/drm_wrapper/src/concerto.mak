ifneq ($(TARGET_PLATFORM),PC)

ifeq ($(TARGET_OS),$(filter $(TARGET_OS), LINUX))

include $(PRELUDE)

TARGET      := app_utils_drm_wrapper
TARGETTYPE  := library

CSOURCES    := drm_wrapper.c

IDIRS += $(LINUX_FS_PATH)/usr/include/
IDIRS += $(LINUX_FS_PATH)/usr/include/libdrm/
IDIRS += $(LINUX_FS_PATH)/usr/include/drm/
IDIRS += $(TIOVX_PATH)/include/
IDIRS += $(VISION_APPS_PATH)/

include $(FINALE)

endif

endif
