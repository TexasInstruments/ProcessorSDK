# Filename: Makefile
#
# This file is the makefile for building the voltage read example.
#
#   Copyright (C) 2018 - 2019 Texas Instruments Incorporated
#
#******************************************************************************

ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = pm_baremetal_voltage_read_testapp
BUILD_OS_TYPE=baremetal

SRCDIR = .
INCDIR = .

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk
INCLUDE_INTERNAL_INTERFACES = pm_example_utils

# List all the components required by the application
COMP_LIST_COMMON = pm_example_utils

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

# Common source files and CFLAGS across all platforms and cores
ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200))
SRCS_COMMON += main_dmsc.c
COMP_LIST_COMMON += $(PDK_COMMON_BAREMETAL_COMP)
 ifeq ($(CORE),mpu1_0)
     LNKFLAGS_LOCAL_mpu1_0 += --entry Entry  --specs=rdimon.specs
 endif
else
COMP_LIST_COMMON += csl pm_lib
COMP_LIST_a15_0 = csl_init
endif
PACKAGE_SRCS_COMMON = .

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
