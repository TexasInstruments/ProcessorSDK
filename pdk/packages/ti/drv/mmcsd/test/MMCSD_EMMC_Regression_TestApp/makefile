# Makefile for EMMC DMA unit test app
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

# Assumes - this would not be invoked for baremetal - refer mmcsd_component.mk

#FreeRTOS Specifics
ifeq ($(BUILD_OS_TYPE), freertos)

    APP_NAME = MMCSD_EMMC_Regression_TestApp_$(BUILD_OS_TYPE)
    CFLAGS_OS_DEFINES = -DFREERTOS -DRTOS_ENV
    INCLUDE_EXTERNAL_INTERFACES = freertos pdk
    COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)

endif


SRCDIR = ../src ../../
INCDIR = ../src ../include ../../
# Common source files across all platforms and cores
SRCS_COMMON += main_emmc.c mmc_profiling_test.c


# List all the components required by the application
COMP_LIST_COMMON += mmcsd

CFLAGS_MMCSD_UT += -DMMCSD_ADMA_ENABLED -DMMCSD_REGRESSION_TEST -DMEASURE_TIME
#CFLAGS_MMCSD_UT +=-DMMCSD_TEST_DDR_DISABLED

#CFLAGS_MMCSD_UT += -DSTRESS_PATTERN_RANDOM_SMH_TEST_ENABLE
#SRCS_COMMON += stress_test_pattern.c
#CFLAGS_MMCSD_UT += -DSTRESS_PATTERN_TEST_ENABLE
#SRCS_COMMON += random_test_pattern.c

# CFLAGS_MMCSD_UT += -DIO_CONSOLE

PACKAGE_SRCS_COMMON = ../
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_MMCSD_UT) $(CFLAGS_OS_DEFINES)

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

# Include common make files
ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
