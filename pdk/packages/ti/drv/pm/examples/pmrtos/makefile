# Filename: Makefile
#
# This file is the makefile for building the pm rtos example.
#
#   Copyright (C) 2015 - 2017 Texas Instruments Incorporated
#
#******************************************************************************
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

APP_NAME = pm_rtos_testapp

SRCDIR = src
INCDIR = . src

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk pm_example_utils pm_rtos bios xdc

# List all the components required by the application
COMP_LIST_COMMON = pm_rtos pm_example_utils bios xdc
ifeq ($(SOC),$(filter $(SOC), am574x am572x am571x am65xx))
COMP_LIST_COMMON += $(PDK_COMMON_TIRTOS_COMP)
else
COMP_LIST_COMMON += csl osal_tirtos
ifeq ($(ISA),$(filter $(ISA), r5f))
  COMP_LIST_COMMON += copyvecs
endif
endif

# Enable XDC build for application by providing XDC CFG File per core
XDC_CFG_FILE_ipu1_0 = $(PDK_PM_COMP_PATH)/examples/pmrtos/pm.cfg
XDC_CFG_FILE_a15_0 = $(PDK_PM_COMP_PATH)/examples/pmrtos/pm_a15.cfg
XDC_CFG_FILE_c66x = $(PDK_PM_COMP_PATH)/examples/pmrtos/pm_c66.cfg

ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu1_1))
# Enable XDC build for application by providing XDC CFG File per core
XDC_CFG_FILE_$(CORE) = $(PDK_PM_COMP_PATH)/examples/pmrtos/pm_r5f.cfg
endif

ifeq ($(CORE),$(filter $(CORE), mpu1_0))
# Enable XDC build for application by providing XDC CFG File per core
XDC_CFG_FILE_$(CORE) = $(PDK_PM_COMP_PATH)/examples/pmrtos/pm_a53.cfg
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

# Common source files and CFLAGS across all platforms and cores
ifeq ($(SOC),$(filter $(SOC), am574x tda2xx tda2px am572x dra75x tda2ex am571x tda3xx dra78x))
SRCS_COMMON += pm_main_prcm.c
endif
ifeq ($(SOC),$(filter $(SOC), am65xx))
SRCDIR += ../utils
SRCS_COMMON += pm_main_dmsc.c
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
