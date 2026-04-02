ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), A72 A53))

include $(PRELUDE)

CSOURCES    := app_heterogeneous.c
TARGET      := vx_app_heterogeneous
TARGETTYPE  := exe

include $(VISION_APPS_PATH)/apps/concerto_mpu_inc.mak

ifeq ($(TARGET_OS), $(filter $(TARGET_OS), LINUX QNX))
STATIC_LIBS += $(IMAGING_LIBS)
endif

IDIRS += $(IMAGING_IDIRS)

ifneq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j722s))
SKIPBUILD=1
endif

include $(FINALE)

endif