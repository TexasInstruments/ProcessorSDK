ifeq ($(TARGET_PLATFORM), $(filter $(TARGET_PLATFORM), J721E J721S2 J784S4 J742S2))
ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), R5F))
ifeq ($(BUILD_VIDEO_IO_KERNELS),yes)
ifeq ($(BUILD_DISPLAY_M2M),yes)

include $(PRELUDE)
TARGET      := vx_target_kernels_display_m2m
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)
IDIRS       += $(HOST_ROOT)/kernels/include
IDIRS       += $(HOST_ROOT)/kernels/video_io/include
IDIRS       += $(VXLIB_PATH)/packages
ifeq ($(RTOS_SDK), mcu_plus_sdk)
IDIRS       += $(MCU_PLUS_SDK_PATH)/source
IDIRS       += $(MCU_PLUS_SDK_PATH)/source/drivers
else
IDIRS       += $(PDK_PATH)/packages
IDIRS       += $(PDK_PATH)/packages/ti/drv
endif
IDIRS       += $(TIOVX_PATH)/source/include

include $(FINALE)

endif
endif
endif
endif

