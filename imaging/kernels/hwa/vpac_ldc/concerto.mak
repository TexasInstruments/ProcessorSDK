
ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), X86 x86_64 R5F))
ifeq ($(BUILD_HWA_KERNELS),yes)
ifeq ($(BUILD_VPAC_LDC),yes)

include $(PRELUDE)
TARGET      := vx_target_kernels_vpac_ldc
TARGETTYPE  := library
CSOURCES    := vx_kernels_hwa_target.c vx_vpac_ldc_target_dcc.c

ifeq ($(TARGET_CPU),R5F)
  ifeq ($(BUILD_VLAB),yes)
    CSOURCES    += vx_vpac_ldc_target_sim.c
    IDIRS       += $(VPAC_C_MODELS_PATH)/include
  else
    CSOURCES    += vx_vpac_ldc_target.c
  endif
else
  CSOURCES    += vx_vpac_ldc_target_sim.c vx_vpac_ldc_target_sim_priv.c
  IDIRS       += $(VPAC_C_MODELS_PATH)/include
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

ifeq ($(TARGET_CPU)$(BUILD_VLAB),R5Fyes)
DEFS += VLAB_HWA
endif

ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), x86_64))
DEFS        += __aarch64__
endif

include $(FINALE)

endif
endif
endif
