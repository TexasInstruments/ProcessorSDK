#
# This file is the makefile for building safety checkers library.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

MODULE_NAME = safety_checkers

SOC_DIR=$(SOC)
ifeq ($(SOC), j742s2)
  SOC_DIR=j784s4
endif

SRCDIR = .
SRCDIR += soc/$(SOC_DIR)/
INCDIR = . soc/ soc/$(SOC_DIR)/
INCDIR += $(PDK_INSTALL_PATH)/ti/csl/ $(PDK_INSTALL_PATH)/ti/csl/soc/$(SOC_DIR)/src/
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/sciclient
INCDIR += $(PDK_INSTALL_PATH)/ti/drv/csirx/src $(PDK_INSTALL_PATH)/ti/drv/csirx/soc/V0

INCLUDE_EXTERNAL_INTERFACES = pdk
INCLUDE_INTERNAL_INTERFACES = pdk csl

SRCS_COMMON += safety_checkers_pm.c safety_checkers_rm.c safety_checkers_tifs.c 

ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
SRCS_COMMON += safety_checkers_csirx.c safety_checkers_csirx_soc.c
endif
ifeq ($(SOC),$(filter $(SOC), j7200 j721e j721s2 j784s4 j742s2))
PACKAGE_SRCS_COMMON += makefile soc/safety_checkers_pm_soc.h soc/safety_checkers_rm_soc.h  soc/$(SOC_DIR)
PACKAGE_SRCS_COMMON += safety_checkers_pm.c safety_checkers_pm.h safety_checkers_rm.c safety_checkers_rm.h safety_checkers_tifs.c safety_checkers_tifs.h safety_checkers_common.h 
PACKAGE_SRCS_COMMON += ../safety_checkers_component.mk
endif

ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
PACKAGE_SRCS_COMMON += soc/safety_checkers_csirx_soc.h soc/$(SOC_DIR)/safety_checkers_csirx_soc.c soc/$(SOC_DIR)/safety_checkers_csirx_soc.h safety_checkers_csirx.h safety_checkers_csirx.c
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
