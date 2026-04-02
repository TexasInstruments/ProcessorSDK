#
# This file is the makefile for building TIFS safety checkers tests
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = tifs_checkers_app_$(BUILD_OS_TYPE)

SOC_DIR=$(SOC)
ifeq ($(SOC), j742s2)
  SOC_DIR=j784s4
endif

SRCDIR = .

INCDIR += . $(PDK_INSTALL_PATH)/ti/safety_checkers/src
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient/
INCDIR += $(PDK_INSTALL_PATH)/ti/csl/
INCDIR += $(PDK_INSTALL_PATH)/ti/csl/soc/$(SOC_DIR)/src/
INCDIR += soc/$(SOC_DIR)/ ../../src/soc/ ../../src/soc/$(SOC_DIR)/

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

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
SRCS_COMMON += safety_checkers_tifs_app.c safety_checkers_tifs_app_main.c

PACKAGE_SRCS_COMMON = makefile safety_checkers_tifs_app.c safety_checkers_tifs_app_main.c soc/$(SOC_DIR)/tifs_checkers_fwl_config.h soc/$(SOC_DIR)/tifs_checkers_isc_config.h

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
