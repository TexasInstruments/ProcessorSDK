#
# This file is the makefile for building CSL timer app.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

#Name of the directory created under packages/ti/binary/
APP_NAME = osal_baremetal_testapp
BUILD_OS_TYPE = baremetal

SRCDIR = ../src
INCDIR = ../src

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk csl_arch

# List all the components required by the application
COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = . ../src
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) -DBARE_METAL

ifeq ($(MANUAL_CACHE_TEST),YES)
  CFLAGS_LOCAL_COMMON += -DMANUAL_CACHE_TEST
endif

ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
  CFLAGS_LOCAL_COMMON += -DENABLE_TIMER_TEST
endif

SRCS_COMMON += main_osal_test.c

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
