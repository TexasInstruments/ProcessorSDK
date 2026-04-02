TIDL_TB_FILES :=
COMMON_FILE   :=

# This is relative to the plat directory
TIDL_TB_FILES += tidl_tb.c
TIDL_TB_FILES += tidl_tb_utils.c
TIDL_TB_FILES += tidl_config.c
TIDL_TB_FILES += tidl_image_postproc.c
TIDL_TB_FILES += tidl_image_preproc.c
TIDL_TB_FILES += tidl_image_read_write.c
TIDL_TB_FILES += ti_draw_utils.c
TIDL_TB_FILES += ti_mem_manager.c
TIDL_TB_FILES += configparser.c

CSOURCES += $(foreach file, $(TIDL_TB_FILES), ../src/$(file))

# include search directories needed by all platforms
ifeq ($(RTOS_SDK),mcu_plus_sdk)
    DEFS+=MCU_PLUS_SDK
    IDIRS += $(DMA_UTILS_PATH)
    IDIRS += $(MCU_PLUS_SDK_PATH)/source
else
    IDIRS += $(PDK_PATH)
endif
IDIRS += $(IVISION_PATH)
IDIRS += $($(_MODULE)_SDIR)/../src
IDIRS += $($(_MODULE)_SDIR)/../../inc

# Note: this is linked as shared library. that means you will need to copy
# the build .so file to the target device (PC or EVM)
LDIRS += $(VISION_APPS_PATH)/out/$(TARGET_PLATFORM)/$(TARGET_CPU)/LINUX/$(TARGET_BUILD)
SHARED_LIBS += vx_tidl_rt

# This is needed for tivision_apps (if it is calling this makefile as part of build)
$(_MODULE)_LOPT += -rpath-link=$(VISION_APPS_PATH)/out/$(TARGET_OVX_PATH)/$(TARGET_CPU)/$(TARGET_OS)/$(TARGET_BUILD)


# defs needed by all platforms
ifeq ($(TARGET_SOC),$(filter $(TARGET_SOC), J721E j721e))
    CFLAGS += -DSOC_J721E
endif
ifeq ($(TARGET_SOC),$(filter $(TARGET_SOC), J721S2 j721s2))
    CFLAGS += -DSOC_J721S2
endif
ifeq ($(TARGET_SOC),$(filter $(TARGET_SOC), J784S4 j784s4))
    CFLAGS += -DSOC_J784S4
else ifeq ($(TARGET_SOC),$(filter $(TARGET_SOC), AM62A am62a))
    CFLAGS += -DSOC_AM62A
    CFLAGS += -DSOC_AM62AX
endif
ifeq ($(TARGET_SOC),$(filter $(TARGET_SOC), J722S j722s))
    CFLAGS += -DSOC_J722S
endif
ifeq ($(TARGET_SOC),$(filter $(TARGET_SOC), J742S2 j742s2))
    CFLAGS += -DSOC_J742S2
endif

DEFS+=BUILD_C7X_1
DEFS+=TIDLRT_BUILD

ifeq ($(CODE_COVERAGE_ENABLED_FOR_TIDL), yes)
DEFS+= CODE_COVERAGE_ENABLED_FOR_TIDL
endif
