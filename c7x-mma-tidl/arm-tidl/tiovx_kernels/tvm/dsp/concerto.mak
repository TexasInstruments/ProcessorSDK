ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), C71 C7120 C7504 C7524))

include $(PRELUDE)
TARGET      := vx_target_kernels_tvm
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)
IDIRS       += $(VXLIB_PATH)/packages
IDIRS       += $(TIOVX_PATH)/source/include
IDIRS       += $(TIOVX_PATH)/include
IDIRS       += $(TIOVX_PATH)/kernels/include
IDIRS       += $(TIDL_TIOVX_KERNELS_PATH)/include
ifeq ($(RTOS_SDK), mcu_plus_sdk)
	IDIRS       += $(MCU_PLUS_SDK_PATH)/source
	IDIRS       += $(MCU_PLUS_SDK_PATH)/source/kernel/dpl
else
	IDIRS       += $(PDK_PATH)
	IDIRS       += $(PDK_PATH)/ti/osal
endif

include $(FINALE)

endif
