ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), A72 A53))
ifeq ($(TARGET_OS), $(filter $(TARGET_OS), LINUX QNX))

include $(PRELUDE)

TARGETTYPE  := exe
TARGET      := vx_app_tutorial
CSOURCES    := $(call all-c-files)

include $(VISION_APPS_PATH)/apps/concerto_mpu_inc.mak

STATIC_LIBS += $(TEST_LIBS)

include $(FINALE)

endif
endif
