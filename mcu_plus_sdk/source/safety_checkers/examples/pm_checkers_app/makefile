#
# Makefile for building PM Safety Checkers app
#

ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

ifeq ($(BUILD_OS_TYPE), baremetal)
    APP_NAME = pm_checkers_app_$(BUILD_OS_TYPE)
else
    APP_NAME = pm_checkers_app_$(BUILD_OS_TYPE)
endif

SOC_DIR=$(SOC)
ifeq ($(SOC), j742s2)
  SOC_DIR=j784s4
endif

SRCDIR = .
INCDIR += . $(PDK_INSTALL_PATH)/ti/safety_checkers/src
INCDIR += . $(PDK_INSTALL_PATH)/ti/safety_checkers/src/soc
INCDIR += $(PDK_INSTALL_PATH)/ti/safety_checkers/src/soc/$(SOC_DIR)
INCDIR += $(PDK_INSTALL_PATH)/ti/csl/ $(PDK_INSTALL_PATH)/ti/csl/soc/$(SOC_DIR)/src/

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk sciclient_direct

ifeq ($(BUILD_OS_TYPE), baremetal)
  COMP_LIST_COMMON += $(PDK_COMMON_BAREMETAL_COMP)
endif

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
COMP_LIST_COMMON += safety_checkers

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += pm_checkers_app_main.c pm_checkers_app.c
PACKAGE_SRCS_COMMON = makefile pm_checkers_app_main.c pm_checkers_app.c
CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES)

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
