#
# This file is the makefile for building ddr test app.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make


ifeq ($(BUILD_OS_TYPE),baremetal)
APP_NAME = board_baremetal_ddr_thermal_test_app
else
APP_NAME = board_ddr_thermal_test_app_$(BUILD_OS_TYPE)
endif

ifeq ($(BUILD_OS_TYPE),baremetal)
  COMP_LIST_COMMON    = $(PDK_COMMON_BAREMETAL_COMP)
  CFLAGS_OS_DEFINES   = -DBAREMETAL
endif
ifeq ($(BUILD_OS_TYPE),tirtos)
  CFLAGS_OS_DEFINES = -DUSE_BIOS
  EXTERNAL_INTERFACES = bios xdc
  COMP_LIST_COMMON    = $(PDK_COMMON_TIRTOS_COMP)
  XDC_CFG_FILE_$(CORE) = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sysbios_$(ISA).cfg
endif
ifeq ($(BUILD_OS_TYPE),freertos)
  CFLAGS_OS_DEFINES = -DFREERTOS
  EXTERNAL_INTERFACES = freertos
  COMP_LIST_COMMON    = $(PDK_COMMON_FREERTOS_COMP)
endif
SRCDIR = src
INCDIR = inc

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)

ifneq ($(CSL_BUILD),$(filter $(CSL_BUILD), CSL))
  COMP_LIST_COMMON := $(filter-out csl,$(COMP_LIST_COMMON))
  COMP_LIST_COMMON += csl2 csl_trim
endif

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .
SRCS_COMMON = main.c

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES)

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
