ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), A72 A53))
ifeq ($(TARGET_OS), $(filter $(TARGET_OS), LINUX QNX))

include $(PRELUDE)

TARGET      := vx_app_conformance_core
TARGETTYPE  := exe
CSOURCES    := $(call all-c-files)

include $(VISION_APPS_PATH)/apps/concerto_mpu_inc.mak

STATIC_LIBS += $(IMAGING_LIBS)
STATIC_LIBS += $(TEST_LIBS)

include $(FINALE)

endif
endif

