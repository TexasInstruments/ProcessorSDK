#
# This file is the makefile for building DM power analysis app
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = dm_power_analysis_app_$(BUILD_OS_TYPE)


SRCDIR = .

INCDIR += . $(PDK_INSTALL_PATH)/ti/drv/pdm_utils/ $(PDK_INSTALL_PATH)/ti/drv/pdm_utils/src $(PDK_INSTALL_PATH)/ti/drv/pdm_utils/src/soc/
INCDIR += $(PDK_INSTALL_PATH)/ti/csl/ $(PDK_INSTALL_PATH)/ti/csl/soc/$(SOC_DIR)/src/

ifeq ($(SOC),$(filter $(SOC), j721e))
  SCICLIENT_SOCVER = V1
endif
ifeq ($(SOC),$(filter $(SOC), j7200))
  SCICLIENT_SOCVER = V2
endif
ifeq ($(SOC),$(filter $(SOC), j721s2))
  SCICLIENT_SOCVER = V4
endif
ifeq ($(SOC),$(filter $(SOC), j784s4 j742s2))
  SCICLIENT_SOCVER = V6
endif

ifeq ($(CORE),$(filter $(CORE), mcu1_0))
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient/src/rm_pm_hal/include/
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient/src/rm_pm_hal/pm/
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient/src/rm_pm_hal/pm/include/
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient/soc/$(SCICLIENT_SOCVER)
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient/src/rm_pm_hal/pm/soc/$(SOC_DIR)/include/soc/$(SOC_DIR)/
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient/src/rm_pm_hal/include/soc/$(SOC_DIR)/
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient/src/rm_pm_hal/pm/soc/$(SOC_DIR)/include/
endif

SRCDIR = . src
# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk csl
INCLUDE_INTERNAL_INTERFACES = pdk csl

ifeq ($(BUILD_OS_TYPE),freertos)
  CFLAGS_OS_DEFINES = -DFREERTOS
  INCLUDE_EXTERNAL_INTERFACES += freertos
  COMP_LIST_COMMON += $(PDK_COMMON_FREERTOS_COMP)
endif

# List all the specific components required by the application
COMP_LIST_COMMON += pdm_utils

# External linker command file
EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_INSTALL_PATH)/ti/drv/pdm_utils/examples/dm_power_analysis_app/$(SOC)/linker_$(ISA)_freertos.lds

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += dm_power_analysis_app_main.c dm_power_analysis_app.c

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
