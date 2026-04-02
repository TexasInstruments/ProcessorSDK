# DRM Wrapper Test Application
# This application is only built for AM62A Linux target

ifeq ($(TARGET_PLATFORM), AM62A)
ifeq ($(TARGET_CPU), A53)
ifeq ($(TARGET_OS), LINUX)

include $(PRELUDE)

TARGET      := vx_app_drm_test
TARGETTYPE  := exe

CSOURCES    := main.c

include $(VISION_APPS_PATH)/apps/concerto_mpu_inc.mak

# Link drm_wrapper and libdrm
STATIC_LIBS += app_utils_drm_wrapper
SYS_SHARED_LIBS += drm

IDIRS += $(VISION_APPS_PATH)/

include $(FINALE)

endif
endif
endif
