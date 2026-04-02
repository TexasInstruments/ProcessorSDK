
ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), R5F))
ifeq ($(BUILD_HWA_KERNELS),yes)
ifeq ($(BUILD_VPAC_FC),yes)

include $(PRELUDE)
TARGET      := vx_target_kernels_vpac_fc
TARGETTYPE  := library
CSOURCES    := vx_kernels_hwa_target.c

ifeq ($(TARGET_CPU),R5F)
    CSOURCES    += vx_vpac_fc_target.c vx_vpac_fc_target_drv.c
endif

ifeq ($(RTOS_SDK), mcu_plus_sdk)
  IDIRS       += $(MCU_PLUS_SDK_PATH)/source
  IDIRS       += $(MCU_PLUS_SDK_PATH)/source/drivers
  IDIRS       += $(MCU_PLUS_SDK_PATH)/source/drivers/vhwa
else
  IDIRS       += $(PDK_PATH)/packages
  IDIRS       += $(PDK_PATH)/packages/ti/drv
  IDIRS       += $(PDK_PATH)/packages/ti/drv/vhwa
endif
IDIRS       += $(APP_UTILS_PATH)/

IDIRS       += $(HOST_ROOT)/kernels/include
IDIRS       += $(HOST_ROOT)/kernels/hwa/include
IDIRS       += $(HOST_ROOT)/algos/dcc/include
IDIRS       += $(HOST_ROOT)/algos/awb/include
IDIRS       += $(VXLIB_PATH)/packages

IDIRS       += $(APP_UTILS_PATH)/utils/remote_service/include
IDIRS       += $(APP_UTILS_PATH)/utils/ipc/include

ifeq ($(TARGET_CPU)$(BUILD_VLAB),R5Fyes)
DEFS += VLAB_HWA
endif

ifeq ($(HOST_COMPILER),GCC_LINUX)
CFLAGS += -Wno-unused-result
endif

ifeq ($(RTOS_SDK),mcu_plus_sdk)
CFLAGS+= -DMCU_PLUS_SDK
endif

include $(FINALE)

endif
endif
endif

