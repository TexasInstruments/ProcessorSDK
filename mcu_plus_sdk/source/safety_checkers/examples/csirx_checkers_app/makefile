#
# This file is the makefile for building csirx safety checkers tests
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = csirx_checkers_app_$(BUILD_OS_TYPE)

SOC_DIR=$(SOC)
ifeq ($(SOC), j742s2)
  SOC_DIR=j784s4
endif

SRCDIR = .

INCDIR += . $(PDK_INSTALL_PATH)/ti/safety_checkers/src $(PDK_INSTALL_PATH)/ti/safety_checkers/src/soc $(PDK_INSTALL_PATH)/ti/safety_checkers/src/soc/$(SOC_DIR) $(PDK_INSTALL_PATH)/ti/drv/i2c/
INCDIR += $(PDK_INSTALL_PATH)/ti/csl/ $(PDK_INSTALL_PATH)/ti/csl/soc/$(SOC_DIR)/src/
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/csirx/src $(PDK_INSTALL_PATH)/ti/drv/csirx/soc/V0
SRCDIR = . src src/soc
# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk fvid2 csirx csl
INCLUDE_INTERNAL_INTERFACES = pdk csl

ifeq ($(BUILD_OS_TYPE),freertos)
  CFLAGS_OS_DEFINES = -DFREERTOS
  INCLUDE_EXTERNAL_INTERFACES += freertos
  COMP_LIST_COMMON += $(PDK_COMMON_FREERTOS_COMP)
endif

ifeq ($(BUILD_OS_TYPE),safertos)
  CFLAGS_OS_DEFINES = -DSAFERTOS
  INCLUDE_EXTERNAL_INTERFACES += safertos
  COMP_LIST_COMMON += $(PDK_COMMON_SAFERTOS_COMP)
endif

# List all the specific components required by the application
COMP_LIST_COMMON += safety_checkers fvid2 csirx

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += safety_checkers_app_csirx_main.c safety_checkers_app_csirx.c safety_checkers_app_csirx_sensor.c safety_checkers_app_utility.c
PACKAGE_SRCS_COMMON = . src 

CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES) -DFVID2_CFG_TRACE_ENABLE

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
