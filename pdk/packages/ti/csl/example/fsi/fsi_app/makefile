#
# This file is the makefile for building CSL FSI test app.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

APP_NAME = csl_fsi_test_app
BUILD_OS_TYPE=baremetal

SRCDIR = .
INCDIR =

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
ifeq ($(SOC),$(filter $(SOC), am64x))
  COMP_LIST_COMMON += $(PDK_COMMON_BAREMETAL_COMP)
else
  COMP_LIST_COMMON += csl
endif

COMP_LIST_COMMON += csl_utils_common csl_uart_console

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = . ../common
SRCDIR = . ../common
INCDIR = . ../common
SRCS_COMMON += fsi_app_common.c fsi_app_main.c fsi_app_cpucontrol.c

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

ifeq ($(SOC),$(filter $(SOC), am64x))
 SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src
 SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/config
 INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src
 INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/config
 SRCS_COMMON += unity_config.c unity.c
 CFLAGS_LOCAL_COMMON += -DUNITY_INCLUDE_CONFIG_H
endif

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
