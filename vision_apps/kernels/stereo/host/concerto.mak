
ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), x86_64 A72 A53))

include $(PRELUDE)
TARGET      := vx_kernels_stereo
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)

IDIRS       += $(VISION_APPS_PATH)/kernels/stereo/include
IDIRS       += $(VISION_APPS_PATH)/utils/perception
IDIRS       += $(PTK_PATH)/include

ifeq ($(SOC),$(filter $(SOC), am62a))
SKIPBUILD=1
endif

include $(FINALE)

endif
