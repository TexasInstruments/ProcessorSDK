# This file is the makefile for building CSL R5 CORE example application.
#
#   Copyright (c) Texas Instruments Incorporated 2018-2020
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

APP_NAME = csl_core_r5_baremetal_test_app
BUILD_OS_TYPE=baremetal
SRCDIR = .
INCDIR = .

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
COMP_LIST_COMMON =
ifeq ($(SOC),$(filter $(SOC), am65xx am64x j721e j7200 j784s4 j721s2 j742s2))
  ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu1_1))
    COMP_LIST_COMMON += $(PDK_COMMON_BAREMETAL_COMP) csl_uart_console
  else
    ifeq ($(SOC),$(filter $(SOC), j721e j7200 j784s4 j721s2 j742s2))
      COMP_LIST_COMMON += i2c
    endif
    ifeq ($(CSL_BUILD),$(filter $(CSL_BUILD), CSL))
      COMP_LIST_COMMON += csl
    endif
  endif
endif

ifneq ($(CSL_BUILD),$(filter $(CSL_BUILD), CSL))
  COMP_LIST_COMMON := $(filter-out csl,$(COMP_LIST_COMMON))
  COMP_LIST_COMMON += csl2 csl_trim
endif

ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4))
  SOC_DIR=$(SOC)
else ifeq ($(SOC), $(filter $(SOC), j742s2))
  SOC_DIR=j784s4
endif

ifeq ($(SOC),$(filter $(SOC), j721e j7200 am64x j784s4 j721s2 j742s2))
EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_CSL2_COMP_PATH)/test/core-r5/$(SOC_DIR)/linker_core_r5.lds
endif

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

SRCS_COMMON += core_r5_test.c
SRCS_COMMON += core_r5_hard_err_ctrl_test.c
SRCS_COMMON += core_r5_pmu_test.c
SRCS_COMMON += core_r5_mpu_test.c
SRCS_COMMON += core_r5_rat_test.c
SRCS_COMMON += core_r5_cache_test.c

ifeq ($(SOC),$(filter $(SOC), am65xx am64x j721e j7200 j784s4 j721s2 j742s2))
 SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src
 SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/config
 INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src
 INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/config
 SRCS_COMMON += unity_config.c unity.c
 CFLAGS_LOCAL_COMMON += -DUNITY_INCLUDE_CONFIG_H
endif

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
