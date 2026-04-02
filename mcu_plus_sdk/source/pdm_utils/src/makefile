#
# This file is the makefile for building PDM UTILS library.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

MODULE_NAME = pdm_utils

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

SRCDIR = . soc/$(SOC_DIR)
INCDIR = . .. soc/
INCDIR += $(PDK_INSTALL_PATH)/ti/csl/ $(PDK_INSTALL_PATH)/ti/csl/soc/$(SOC_DIR)/src/
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient/src/rm_pm_hal/include/
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient/src/rm_pm_hal/pm/
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient/src/rm_pm_hal/pm/include/
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient/soc/$(SCICLIENT_SOCVER)
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient/src/rm_pm_hal/pm/soc/$(SOC_DIR)/include/soc/$(SOC_DIR)/
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient/src/rm_pm_hal/include/soc/$(SOC_DIR)/
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient/src/rm_pm_hal/pm/soc/$(SOC_DIR)/include/

INCLUDE_EXTERNAL_INTERFACES = pdk
INCLUDE_INTERNAL_INTERFACES = pdk csl

ifeq ($(SOC),$(filter $(SOC), j784s4 j721s2 j721e j7200))
ifeq ($(CORE), mcu1_0)
SRCS_COMMON += pdm_utils_clock.c
endif
ifneq ($(SOC),$(filter $(SOC),j721e j7200))
SRCS_COMMON += pdm_utils_pet.c
endif
SRCS_COMMON += pdm_utils_pll.c pdm_utils_psc.c pdm_utils_soc.c 
endif

# Package this repo
ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4 j721e j7200))
PACKAGE_SRCS_COMMON  = makefile ../pdm_utils.h
PACKAGE_SRCS_COMMON += pdm_utils_clock.c pdm_utils_pll.c pdm_utils_psc.c
ifneq ($(SOC),$(filter $(SOC),j721e j7200))
PACKAGE_SRCS_COMMON += pdm_utils_pet.c
endif
PACKAGE_SRCS_COMMON += soc/$(SOC_DIR)/pdm_utils_soc.c soc/pdm_utils_soc.h soc/$(SOC_DIR)/pdm_utils_soc.h
PACKAGE_SRCS_COMMON += ../examples/dm_power_analysis_app/makefile
PACKAGE_SRCS_COMMON += ../examples/dm_power_analysis_app/src
PACKAGE_SRCS_COMMON += ../examples/dm_power_analysis_app/$(SOC_DIR)/
PACKAGE_SRCS_COMMON += ../pdm_utils_component.mk
endif
# Using this makefile to package the tools as well - we do not have any other build target in PDM Utils
ifeq ($(SOC),$(filter $(SOC), j721s2 j784s4))
PACKAGE_SRCS_COMMON += ../tools/.meta ../tools/.metadata
PACKAGE_SRCS_COMMON += ../tools/config/.meta/$(SOC_DIR)
PACKAGE_SRCS_COMMON += ../tools/dm_power_config_tool/.meta
PACKAGE_SRCS_COMMON += ../tools/dm_power_config_tool/out/$(SOC_DIR)_evm.syscfg
PACKAGE_SRCS_COMMON += ../tools/dm_power_config_tool/makefile
endif
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4))
PACKAGE_SRCS_COMMON += ../tools/jtag_power_analysis_tool/soc_data/common
PACKAGE_SRCS_COMMON += ../tools/jtag_power_analysis_tool/soc_data/$(SOC_DIR)
PACKAGE_SRCS_COMMON += ../tools/jtag_power_analysis_tool/src
PACKAGE_SRCS_COMMON += ../tools/jtag_power_analysis_tool/debugger_scripts
PACKAGE_SRCS_COMMON += ../tools/jtag_power_analysis_tool/doc/sample_output/$(SOC_DIR)
endif

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-nameutils> =
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
