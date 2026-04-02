ifeq ($(TARGET_PLATFORM), PC)

include $(PRELUDE)

CSOURCES    := tidl_rt_x86.c

LDIRS += $(DSP_TOOLS)/host_emulation
LDIRS += $(MMALIB_PATH)/lib/$(C7X_VERSION)/$(TARGET_BUILD)

ifeq ($(TARGET_SOC),$(filter $(TARGET_SOC), AM62A am62a J722S j722s))
    DEFS += DMA_UTILS_STANDALONE
endif

DEFS += HOST_EMULATION

DMA_LIBS =


ifeq ($(RTOS_SDK),mcu_plus_sdk)
    LDIRS += $(DMA_UTILS_PATH)/lib
    ifeq ($(TARGET_SOC),$(filter $(TARGET_SOC), AM62A am62a))
        DMA_LIBS += dmautils.am62ax.c75x.ti-c7x-hostemu.$(TARGET_BUILD).lib
    else ifeq ($(TARGET_SOC),$(filter $(TARGET_SOC), J722S j722s))
        DMA_LIBS += dmautils.j722s.c75ssx-0.ti-c7x-hostemu.$(TARGET_BUILD).lib
    endif
else
    LDIRS += $(PDK_PATH)/ti/csl/lib/$(SOC)/c7x-hostemu/$(TARGET_BUILD)
    LDIRS += $(PDK_PATH)/ti/osal/lib/nonos/$(SOC)/c7x-hostemu/$(TARGET_BUILD)
    LDIRS += $(PDK_PATH)/ti/drv/sciclient/lib/$(SOC)_hostemu/c7x-hostemu/$(TARGET_BUILD)
    LDIRS += $(PDK_PATH)/ti/drv/udma/lib/$(SOC)_hostemu/c7x-hostemu/$(TARGET_BUILD)
    DMA_LIBS += dmautils.lib
    DMA_LIBS += ti.csl.lib
    ifeq ($(ENABLE_SDK_9_2_COMPATIBILITY), 1)    
    else
        DMA_LIBS += ti.csl.init.lib
    endif    
    ifeq ($(TARGET_SOC),$(filter $(TARGET_SOC), AM62A am62a))
    else
        DMA_LIBS += udma.lib
        DMA_LIBS += sciclient.lib
        DMA_LIBS += ti.osal.lib
    endif
endif

ifeq ($(TIDL_BUILD_PATHS), LEGACY)
    LDIRS += $(TIDL_PATH)/ti_dl/lib/PC/dsp/algo/$(TARGET_BUILD)
else
    LDIRS += $(TIDL_PATH)/ti_dl/lib/$(TARGET_SOC)/PC/algo/$(TARGET_BUILD)
endif
LDIRS += $(TIOVX_PATH)/lib/PC/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)
LDIRS += $(TIDL_PATH)/arm-tidl/tiovx_kernels/lib/PC/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)
LDIRS += $(VISION_APPS_PATH)/lib/PC/$(TARGET_OVX_PATH)/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)
LDIRS += $(APP_UTILS_PATH)/lib/PC/$(TARGET_OVX_PATH)/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)

ifeq ($(GCOV_ENABLED), 1)
    $(_MODULE)_COPT += -fprofile-arcs
    $(_MODULE)_COPT += -ftest-coverage
endif

MMA_LIBS =
MMA_LIBS += mmalib_cn_x86_64
MMA_LIBS += mmalib_x86_64
MMA_LIBS += common_x86_64

TIDL_LIBS =
TIDL_LIBS += tidl_algo
TIDL_LIBS += tidl_obj_algo
TIDL_LIBS += tidl_priv_algo
TIDL_LIBS += tidl_custom

TIDL_LIBS += tidl_avx_kernels

TIOVX_LIBS  =
TIOVX_LIBS += vx_framework
TIOVX_LIBS += vx_platform_pc
TIOVX_LIBS += vx_kernels_host_utils vx_kernels_target_utils
TIOVX_LIBS += vx_kernels_tidl vx_nested_kernels_tidl
TIOVX_LIBS += vx_kernels_openvx_core vx_target_kernels_openvx_core vx_kernels_openvx_ext vx_target_kernels_openvx_ext
TIOVX_LIBS += vx_utils
TIOVX_LIBS += vx_target_kernels_tidl
TIOVX_LIBS += vx_target_kernels_ivision_common
TIOVX_LIBS += vx_target_kernels_dsp
TIOVX_LIBS += vx_target_kernels_source_sink
TIOVX_LIBS += vx_target_kernels_tutorial vx_kernels_test_kernels

VISION_APPS_UTILS_LIBS  =
VISION_APPS_UTILS_LIBS += app_utils_mem
VISION_APPS_UTILS_LIBS += app_utils_init
VISION_APPS_UTILS_LIBS += app_utils_file_io

ADDITIONAL_STATIC_LIBS += $(DMA_LIBS)

STATIC_LIBS += $(TIDL_LIBS)
STATIC_LIBS += $(MMA_LIBS)
STATIC_LIBS += $(TIOVX_LIBS)
STATIC_LIBS += $(VISION_APPS_UTILS_LIBS)
STATIC_LIBS += vxlib_$(TARGET_CPU) c6xsim_$(TARGET_CPU)_C66
STATIC_LIBS += $(C7X_VERSION)$(C7x_HOSTEMU_COMPILER_STRING)-host-emulation

SHARED_LIBS += rt

include $($(_MODULE)_SDIR)/../concerto_common.mak

include $(FINALE)

endif
