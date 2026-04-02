
ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), X86 x86_64 C71 C7120 C7504 C7524))

include $(PRELUDE)
TARGET      := vx_target_kernels_tidl
TARGETTYPE  := library
CSOURCES    := $(call all-c-files)
IDIRS       += $(TIOVX_PATH)/kernels/ivision/include
IDIRS       += $(TIOVX_PATH)/kernels/include
IDIRS       += $(TIOVX_PATH)/include
IDIRS       += $(TIDL_TIOVX_KERNELS_PATH)/tidl/include
IDIRS       += $(TIDL_TIOVX_KERNELS_PATH)/include
IDIRS       += $(IVISION_PATH)
IDIRS       += $(TIDL_PATH)/ti_dl/inc
IDIRS       += $(TIDL_PATH)/ti_dl/custom
IDIRS       += $(VXLIB_PATH)/packages
IDIRS       += $(APP_UTILS_PATH)/utils/file_io/include
IDIRS       += $(APP_UTILS_PATH)/utils/udma/include

LDIRS       = $(APP_UTILS_PATH)/lib/$(TARGET_SOC)/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)
SYS_STATIC_LIBS += app_utils_file_io

ifeq ($(RTOS_SDK), mcu_plus_sdk)
  IDIRS       += $(MCU_PLUS_SDK_PATH)/source
  IDIRS       += $(MCU_PLUS_SDK_PATH)/source/drivers
  IDIRS       += $(MCU_PLUS_SDK_PATH)/source/kernel/dpl
  DEFS += MCU_PLUS_SDK
else
  IDIRS       += $(PDK_PATH)
  IDIRS       += $(PDK_PATH)/ti/drv
  IDIRS       += $(PDK_PATH)/ti/drv/udma
  IDIRS       += $(PDK_PATH)/ti/osal
endif

ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), X86 x86_64))
CFLAGS      += -D_HOST_BUILD -D_TMS320C6600 -DTMS320C66X -DHOST_EMULATION -D_TMS320C6X

ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), x86_64))
DEFS        += __aarch64__
endif

ifeq ($(CODE_COVERAGE_ENABLED_FOR_RT), yes)
CFLAGS+= --no-warnings
endif

ifeq ($(GCOV_ENABLED), 1)
    $(_MODULE)_COPT += -fprofile-arcs
    $(_MODULE)_COPT += -ftest-coverage
endif

endif

include $(FINALE)

endif
