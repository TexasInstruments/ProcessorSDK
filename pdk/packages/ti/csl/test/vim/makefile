# This file is the makefile for building CSL VIM example application.
#
#   Copyright (c) 2018-2022 Texas Instruments Incorporated
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

APP_NAME = csl_vim_baremetal_test_app
BUILD_OS_TYPE=baremetal
SRCDIR = .
INCDIR = .

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
COMP_LIST_COMMON =
ifeq ($(SOC),$(filter $(SOC), am65xx am64x j721e j7200 j721s2 j784s4 j742s2))
  ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu1_1))
    COMP_LIST_COMMON += $(PDK_COMMON_BAREMETAL_COMP) csl_uart_console
  else
    ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
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

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

SRCS_COMMON += vim_test_main.c vim_test_interrupt.c

ifeq ($(SOC),$(filter $(SOC), am65xx j721e am64x j7200 j721s2 j784s4 j742s2))
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
