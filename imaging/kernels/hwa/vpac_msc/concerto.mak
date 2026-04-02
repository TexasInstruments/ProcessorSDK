
ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), X86 x86_64 R5F))
ifeq ($(BUILD_HWA_KERNELS),yes)
ifeq ($(BUILD_VPAC_MSC),yes)

include $(PRELUDE)
TARGET      := vx_target_kernels_vpac_msc
TARGETTYPE  := library
ifeq ($(TARGET_CPU),R5F)
  ifeq ($(BUILD_VLAB),yes)
    CSOURCES    := vx_vpac_msc_scale_target_sim.c vx_vpac_msc_pyramid_target_sim.c vx_vpac_msc_halfscalegaussian_target_sim.c vx_vpac_msc_multi_scale_output_target_sim.c
    IDIRS       += $(VPAC_C_MODELS_PATH)/include
  else
    CSOURCES    := vx_vpac_msc_multi_scale_output_target.c vx_vpac_msc_pyramid_target.c vx_vpac_msc_scale_target.c
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
  endif
else
  CSOURCES    := vx_vpac_msc_scale_target_sim.c vx_vpac_msc_pyramid_target_sim.c vx_vpac_msc_halfscalegaussian_target_sim.c vx_vpac_msc_multi_scale_output_target_sim.c
  IDIRS       += $(VPAC_C_MODELS_PATH)/include
endif

CSOURCES    += vx_kernels_hwa_target.c
IDIRS       += $(HOST_ROOT)/kernels/include
IDIRS       += $(HOST_ROOT)/kernels/hwa/include
IDIRS       += $(VXLIB_PATH)/packages
IDIRS       += $(TIOVX_PATH)/kernels/openvx-core/include

# Note: suppressing this warning to enable cleaner code in kernel registration for portability across SoC's
CFLAGS += -Wno-parentheses-equality

ifeq ($(TARGET_CPU)$(BUILD_VLAB),R5Fyes)
DEFS += VLAB_HWA
endif

include $(FINALE)

endif
endif
endif
