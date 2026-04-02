# Makefile for MMCSD DMA unit test app
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

export DISABLE_RECURSE_DEPS
BUILD_OS_TYPE = baremetal
#Name of the directory created under packages/ti/binary/
APP_NAME = MMCSD_EMMC_Baremetal_Regression_TestApp

ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
SRCDIR = ../src ../../
INCDIR = ../src ../include ../../
# Common source files across all platforms and cores
SRCS_COMMON += main_emmc.c
CFLAGS_MMCSD_UT += -DMMCSD_ADMA_ENABLED -DMMCSD_REGRESSION_TEST
#CFLAGS_MMCSD_UT += -DMMCSD_TEST_DDR_DISABLED

#CFLAGS_MMCSD_UT += -DSTRESS_PATTERN_RANDOM_SMH_TEST_ENABLE
#SRCS_COMMON += stress_test_pattern.c

#CFLAGS_MMCSD_UT += -DSTRESS_PATTERN_TEST_ENABLE
#SRCS_COMMON += random_test_pattern.c

#CFLAGS_MMCSD_UT += -DSTRESS_TEST_ENABLED

endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk csl_arch

# List all the components required by the application
COMP_LIST_COMMON += $(PDK_COMMON_BAREMETAL_COMP)
COMP_LIST_COMMON += mmcsd

# CFLAGS_MMCSD_UT += -DIO_CONSOLE

PACKAGE_SRCS_COMMON = ../
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_MMCSD_UT) -DBARE_METAL

LNKFLAGS_LOCAL_mpu1_0 += --entry Entry

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
